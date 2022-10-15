#ifndef MOZI_CORE_CLASS_LOADER_COMPONENT_FACTORY_H
#define MOZI_CORE_CLASS_LOADER_COMPONENT_FACTORY_H

#include "component_base.h"

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

class ComponentBase;

using CreateCallback = std::function<ComponentBase*()>;
using ComponentCallbackMap = std::map<std::string, CreateCallback>;

class ComponentFactory
{
	public:
	static void RegisterComponent(const std::string& component_name,
			CreateCallback create_cb);
	static void UnregisterComponent(const std::string& component_name);
	static ComponentBase* CreateSingleComponent(const std::string& component_name);
	private:
	static ComponentCallbackMap components_map_;
};

#endif
