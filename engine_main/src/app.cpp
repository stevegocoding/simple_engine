#include "app.h"
#include "factory_manager.h"
#include "render_system.h"

App* App::m_instance = NULL;

App::App(const CreationSettings& config) 
	: m_is_running(false) 
	, m_settings(config)
	, m_render_system(NULL)
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




/*
AppImplPtr App::_create_impl()
{
	return AppImplPtr(); 
}
*/