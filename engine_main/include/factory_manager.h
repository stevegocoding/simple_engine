#ifndef __FACTORY_MANAGER_H__
#define __FACTORY_MANAGER_H__

#include <map>

#include "prerequisites.h"
#include "utils.h"
#include "render_system.h"

class FactoryManager : public Singleton<FactoryManager>
{
public:
	FactoryManager(); 
	~FactoryManager();

	void initialize(); 
	void shutdown(); 
	
	RenderSystem* create_render_system(RS_TYPE rs_type); 
	
private:
	RSFactoryPtr get_rs_factory(RS_TYPE rs_type); 

	typedef std::map<RS_TYPE, RSFactoryPtr> RSFactroyMap; 
	RSFactroyMap m_rs_factroies; 
};


#endif

