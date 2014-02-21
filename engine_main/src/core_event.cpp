#include "core_event.h"
#include "input_event.h"

EventManager::EventManager()
	: m_listener_order(0)
{
	// Mouse events
	register_system_event<MouseEvent>(IET_MOUSE_BTN_DOWN, "glf::IET_MOUSE_BTN_DOWN");
	register_system_event<MouseEvent>(IET_MOUSE_BTN_UP, "glf::IET_MOUSE_BTN_UP");
	register_system_event<MouseEvent>(IET_MOUSE_MOVED, "glf::IET_MOUSE_MOVED");
	register_system_event<MouseEvent>(IET_MOUSE_WHEEL, "glf::IET_MOUSE_WHEEL");

	//Touchpad events
	register_system_event<TouchPadEvent>(IET_TOUCHPAD_BEGAN, "glf::IET_TOUCHPAD_BEGAN");
	register_system_event<TouchPadEvent>(IET_TOUCHPAD_MOVED, "glf::IET_TOUCHPAD_MOVED");
	register_system_event<TouchPadEvent>(IET_TOUCHPAD_ENDED, "glf::IET_TOUCHPAD_ENDED");	
	
}

EventManager::~EventManager()
{
	
}

void EventManager::add_listener(EventListener *listener, int priority)
{
	EventListenerData *listener_data = NULL; 
	for (EventListenerDataList::iterator it = m_event_listeners.begin(); 
		it != m_event_listeners.end(); ++it)
	{
		if ((*it).listener == listener)
		{
			// Already Existed
			listener_data = &*it; 
			break; 
		}
	}
	
	if (!listener_data)
	{
		// Create a new one
		m_event_listeners.resize(m_event_listeners.size()+1); 
		listener_data = &m_event_listeners.back();
	}

	listener_data->listener = listener; 
	listener_data->order = m_listener_order++; 
	listener_data->priority = priority; 
	
	m_event_listeners.sort(); 
}


void EventManager::post_event(const CoreEvent& event)
{
	std::map<int, TypeInfo>::const_iterator it = m_event_infos.find(event.type); 
	
	assert(it != m_event_infos.end() && "This event has not been registered!");
	
	CoreEvent::EventData data;
	memcpy(&data, &event, it->second.m_size); 
	
	m_events_queue.push(data); 
}


void EventManager::dispatch_events()
{
	CoreEvent::EventData data; 
	while(!m_events_queue.empty())
	{
		data = m_events_queue.front();
		m_events_queue.pop();

		// Raise event
		CoreEvent *event = reinterpret_cast<CoreEvent*>(&data);
		_raise_event(*event); 
	}
}

bool EventManager::presend_event(CoreEvent& event)
{
	switch (event.type)
	{
		// Mouse
	case IET_MOUSE_BTN_DOWN: 
	case IET_MOUSE_BTN_UP:
	case IET_MOUSE_MOVED: 
		InputEvent &input_evt = static_cast<InputEvent&>(event); 
		return input_evt.m_input_device->presend_event(input_evt); 
	}
	
	return true; 
}

bool EventManager::_raise_event(CoreEvent& evt)
{
	if (!presend_event(evt))
		return false; 
	
	for (EventListenerDataList::const_iterator it = m_event_listeners.begin(); 
		it != m_event_listeners.end(); 
		++it)
	{
		const EventListenerData& eld = *it; 
		EventListener *listener = eld.listener; 
		
		if (listener->is_enabled() && listener->on_event(evt))
		{
			// Return if the event is consumed 
			return true; 
		}
	}

	return false; 
}


void EventManager::_reg_sys_events_type(int type, size_t size, const std::string& event_name)
{
	assert(size <= CoreEvent::MAX_SIZE && "Event size is too big!"); 

	m_event_infos[type].m_name = event_name; 
	m_event_infos[type].m_size = size;
	m_event_infos[type].m_id = (int)m_event_infos.size()-1; 
}

