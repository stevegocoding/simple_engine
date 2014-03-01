#include "input_manager.h"
#include "input_device.h"

InputManager::InputManager()
{
	
}

InputManager::~InputManager()
{
}

void InputManager::add_updated_device(InputDevice *device)
{
	m_update_devices.insert(device); 
}

void InputManager::update_frame()
{
	preupdate_frame(); 

	for (std::set<InputDevice*>::const_iterator it = m_update_devices.begin(); 
		it != m_update_devices.end(); ++it)
	{
		(*it)->reset_frame(); 
	}
	m_update_devices.clear(); 
}	