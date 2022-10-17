#include "component_factory.h"

ComponentFactory::ComponentCallbackMap ComponentFactory::components_map_;

void ComponentFactory::RegisterComponent(
		const std::string& component_name,
		CreateCallback create_cb)
{
	components_map_[componet_name] = create_cb;
}

void ComponentFactory::UnregisterComponent(const std::string& component_name)
{
	auto it = components_map_.find(component_name);
	if (it != components_map_.end())
	{
		componets_map_.erase(it);
	}
}

ComponentBase* ComponentFactory::CreateSingleComponent(const std::string& component_name)
{
	auto it = components_map_.find(component_name);
	if (it != components_map_.end())
	{
		return (it->second)();
	}

	return nullptr;
}
