#include "input_manager_android.h"

InputManagerAndroid::InputManagerAndroid()
{
	
}

InputManagerAndroid::~InputManagerAndroid()
{
	
}

void InputManagerAndroid::preupdate_frame()
{
	m_input.pre_update_frame(); 
}

void InputManagerAndroid::update_frame()
{
	InputManager::update_frame(); 
}

void InputManagerAndroid::InputInfo::pre_update_frame()
{
	
}

void InputManagerAndroid::InputInfo::update_frame()
{
	
}

void InputManagerAndroid::InputInfo::touch_begin(int x, int y)
{
	m_android_ctrl.raise_touch_event(IET_TOUCHPAD_BEGAN, m_current_touch_id, (float)x, (float)y); 
	m_touch_started = true; 
}

void InputManagerAndroid::InputInfo::touch_move(int x, int y)
{
	if (!m_touch_started)
		return; 
	
	m_android_ctrl.raise_touch_event(IET_TOUCHPAD_MOVED, m_current_touch_id, (float)x, (float)y); 
}

void InputManagerAndroid::InputInfo::touch_end(int x, int y, int touch_id /* = -1 */)
{
	m_android_ctrl.raise_touch_event(IET_TOUCHPAD_ENDED, m_current_touch_id, (float)x, (float)y); 
	m_touch_started = false;
}