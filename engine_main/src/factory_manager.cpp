#include "boost/make_shared.hpp"
#include "factory_manager.h"
#include "gles2_render_system.h"

FactoryManager::FactoryManager()
{

}

FactoryManager::~FactoryManager()
{
	
}

void FactoryManager::initialize() 
{
	// Register Render System Factories
	m_rs_factroies.insert(std::make_pair(RS_GLES2, boost::make_shared<GLES2RenderSystemFactory>()));
}

void FactoryManager::shutdown() 
{
	
}

RSFactoryPtr FactoryManager::get_rs_factory(RS_TYPE rs_type)
{
	RSFactroyMap::iterator it = m_rs_factroies.find(rs_type); 
	return (it == m_rs_factroies.end()) ? RSFactoryPtr() : it->second; 
}

RenderSystem* FactoryManager::create_render_system(RS_TYPE rs_type)
{
	RSFactoryPtr factory = get_rs_factory(rs_type);
	return factory->create_render_system(); 
}