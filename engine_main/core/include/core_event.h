#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include <map> 
#include <queue>
#include <list>
#include "prerequisites.h"
#include "types.h"

static const int INPUT_EVENT_TYPE_BASE = 200; 

enum EventTypeEnum
{
	E_FRAME_EVENT = 0
};


struct CoreEvent 
{
	CoreEvent(int type)
		: type((ushort)type)
	{
	}

	/** All events must NOT larger than this */ 
	static const size_t MAX_SIZE = 128; 
	
	typedef struct { char data[MAX_SIZE]; } EventData;
	
	ushort type;  
};


class EventListener 
{
public:

	virtual bool on_event(const CoreEvent& e) = 0;
	virtual bool is_enabled() const = 0; 

};


class EventManager 
{
public:
	EventManager(); 
	virtual ~EventManager();
	
	void add_listener(EventListener *listener, int priority = 0); 
	
	bool presend_event(CoreEvent& event);

	void post_event(const CoreEvent& event); 

	void dispatch_events();

	template <typename T>
	void register_system_event(int type, const std::string& event_name)
	{
		_reg_sys_events_type(type, sizeof(T), event_name);
	}

private:

	bool _raise_event(CoreEvent& evt); 
	
	struct TypeInfo
	{
		TypeInfo() 
			: m_size(0)
			, m_id(-1) 
		{}
		unsigned int m_size;
		int m_id; 
		std::string m_name; 
	};

	struct EventListenerData
	{
		typedef std::vector<bool> BitArray; 
		
		EventListener *listener;
		
		int priority;
		
		int order;
		
		bool operator < (const EventListenerData& other) const 
		{
			return (priority == other.priority)? (order < other.order) : (priority > other.priority); 
		} 
	};

	typedef std::list<EventListenerData> EventListenerDataList; 
	

	void _reg_sys_events_type(int type, size_t size, const std::string& event_name); 
	
	std::map<std::string, int> m_event_types;  

	std::map<int, TypeInfo> m_event_infos;

	std::queue<CoreEvent::EventData> m_events_queue;

	EventListenerDataList m_event_listeners; 

	int m_listener_order; 
};


#endif
