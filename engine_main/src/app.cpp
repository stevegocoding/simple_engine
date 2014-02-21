#include "app.h"
#include "factory_manager.h"
#include "render_system.h"

App* App::m_instance = NULL;

App::App(const CreationSettings& config) 
	: m_is_running(false) 
	, m_settings(config)
	, m_render_system(NULL)
	, m_event_mgr(NULL)
	, m_input_mgr(NULL)
{
	m_instance = this; 
}

App::~App()
{ 
}

int App::init()
{
	//FactoryManager::GetInstance()->initialize(); 
	
	int ret = m_pimp->init(m_settings); 
	return ret;
}

void App::destory()
{ 
	m_pimp->destroy();
	
	// FactoryManager::GetInstance()->shutdown();
}

void App::update()
{
	m_pimp->update(); 
}

void App::render_frame()
{
	
}

void App::present()
{
}

int App::run()
{
	/*
	m_is_running = true; 

	while (m_is_running)
	{
		update(); 
	}

	render_frame(); 
	*/
	
	return 0; 
}


Point<short> App::convert_pos_device_to_screen(const Point<float>& device_pos, bool with_orientation)
{
	
	return Point<short>(); 
}

bool App::on_event(const CoreEvent& e)
{
	return false; 
}


/*
AppImplPtr App::_create_impl()
{
	return AppImplPtr(); 
}
*/