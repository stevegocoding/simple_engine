#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <set>
#include "prerequisites.h"

struct NativeWndMsg
{};

class InputDevice; 
class InputManager 
{
public:
	
	InputManager();
	virtual ~InputManager(); 
	
	virtual void preupdate_frame() = 0; 
	virtual void update_frame(); 
	
	void add_updated_device(InputDevice *device); 

	virtual void on_wnd_events(NativeWndMsg& wnd_msg) = 0; 

protected: 
	std::set<InputDevice*> m_update_devices; 
	
	
};

#endif