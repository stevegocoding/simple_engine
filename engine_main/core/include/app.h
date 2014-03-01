#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <boost/shared_ptr.hpp>
#include <string>
#include "prerequisites.h"
#include "platform.h"
#include "types.h"
#include "core_event.h"

struct CreationSettings 
{
	static const int DEFAULT_VALUE = 0x12345678; 
	
	CreationSettings()
		: wnd_pos_x(DEFAULT_VALUE)
		, wnd_pos_y(DEFAULT_VALUE)
		, wnd_width(DEFAULT_VALUE)
		, wnd_height(DEFAULT_VALUE)
		, wnd_title("AppWindow")
		, depthbuf_size(16)
		, pixel_size(16)
		, wait_vsync(false)
	{
	}
	
	/** Window Settings */
	int wnd_pos_x; 
	int wnd_pos_y;
	unsigned int wnd_width; 
	unsigned int wnd_height; 
	std::string wnd_title;
	
	/** Rendering Settings */ 
	int depthbuf_size;
	int pixel_size; 
	bool wait_vsync; 
};
 

struct PlatformInfo
{
};
typedef boost::shared_ptr<PlatformInfo> platforminfo_ptr; 

class App;
struct AppImplBase
{
public:
	AppImplBase(App& app)
		: m_app(app)
	{}

	virtual ~AppImplBase() 
	{}

	virtual int init(const CreationSettings& settings) = 0; 
	virtual void destroy() = 0;
	virtual void update() = 0; 

protected: 	
	App& m_app; 
}; 
typedef boost::shared_ptr<AppImplBase> appimpl_ptr;


class App; 
struct GlobalsBase
{
	GlobalsBase()
		: app(NULL) 
	{} 

	virtual ~GlobalsBase()
	{
	}
 
	App *app; 
};
typedef boost::shared_ptr<GlobalsBase> GlobalsBasePtr; 

void init_globals(); 
GlobalsBasePtr get_globals();
void destroy_globals(); 

App *get_app(); 


class EventManager; 
class InputManager; 
class App : public EventListener
{ 
public:
	App(const CreationSettings& config);
	virtual ~App(); 

	virtual int init();
	virtual void destroy();
	virtual int run();
	virtual void update();
	virtual void render_frame(); 
	virtual void present(); 

	bool is_running() const { return m_is_running; } 
	platforminfo_ptr get_platform_info() { return m_platform_info; }
	EventManager* get_event_mgr() { return m_event_mgr; }
	InputManager* get_input_mgr() { return m_input_mgr; }

	Point<short> convert_pos_device_to_screen(const Point<float>& device_pos, 
		bool with_orientation = true); 
	
	virtual bool is_enabled() const { return true; }
	virtual bool on_event(const CoreEvent& e); 

protected: 
	
	virtual platforminfo_ptr _create_platform_info() = 0; 
	
	static App* m_instance; 

	bool m_is_running; 
	appimpl_ptr m_pimp; 
	CreationSettings m_settings;
	platforminfo_ptr m_platform_info;

	/** Render System */
	RenderSystem *m_render_system;
	
	/** Event Manager */
	EventManager *m_event_mgr;

	/** Input Manager */
	InputManager *m_input_mgr; 
}; 

App* CreateApp(); 

App* GetApp(); 


#endif
