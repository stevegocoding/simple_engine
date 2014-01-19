#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <boost/shared_ptr.hpp>
#include "prerequisites.h"
#include "platform.h"

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
	bool wait_vsync; 
};
 

struct PlatformInfo
{
};
typedef boost::shared_ptr<PlatformInfo> platforminfo_ptr; 

class App;
struct AppImplBase
{
	AppImplBase(App& app)
		: m_app(app)
	{}

	virtual ~AppImplBase() 
	{}

	virtual int init(const CreationSettings& settings) = 0; 
	virtual void destroy() = 0;
	virtual void update() = 0; 
	
	App& m_app; 
}; 
typedef boost::shared_ptr<AppImplBase> AppImplPtr;


class App 
{ 
public:
	App(const CreationSettings& config);
	virtual ~App(); 

	virtual int init();
	virtual void destory();
	virtual int run();
	virtual void update();
	virtual void render_frame(); 
	virtual void present(); 

	bool is_running() const { return m_is_running; }

	static App* GetInstance(); 
	
	AppImplPtr get_pimpl() { return m_pimp; };

	platforminfo_ptr get_platform_info() { return m_platform_info; }

	virtual AppImplPtr _create_impl() = 0;
	
protected: 

	static App* m_instance; 

	bool m_is_running; 
	AppImplPtr m_pimp; 
	CreationSettings m_settings;

	/** Render System */
	RenderSystem *m_render_system; 
	platforminfo_ptr m_platform_info; 
};


App* CreateApp(); 


#endif