#ifndef __INPUT_MANAGER_ANDROID_H__
#define __INPUT_MANAGER_ANDROID_H__

#include "input_manager.h"
#include "input_device.h"

class InputManagerAndroid : public InputManager
{
public:
	
	InputManagerAndroid();
	~InputManagerAndroid();
	
	virtual void preupdate_frame();
	virtual void update_frame(); 
	
	virtual GamePad& get_gamepad(int idx); 
		
private:
	
	struct InputInfo
	{
		InputInfo()
			: m_current_touch_id(0)
			, m_touch_started(false)
		{ 	
		}

		void pre_update_frame();
		void update_frame();

		void touch_begin(int x, int y);
		void touch_move(int x, int y);
		void touch_end(int x, int y, int touch_id = -1); 
		
		AndroidController& get_controller() { return m_android_ctrl; } 
		
		unsigned int m_current_touch_id; 
		bool m_touch_started;
		
		AndroidController m_android_ctrl; 
	};  
	
	InputInfo m_input; 
	
};


#endif 