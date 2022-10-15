#ifndef MOZI_CORE_CLASS_LOADER_REGISTER_MACRO_H
#define MOZI_CORE_CLASS_LOADER_REGISTER_MACRO_H

#include "component_factory.h"

#define REGISTER_COMPONENT(Name CreateCallback, UniqueID) \
	struct Proxy##UniqueID { \
		Proxy##UniqueID() { \
			CoponentFactory::RegisterComponent(Name, CreateCallback); \
		} \
	}; \
	static Proxy##UniqueID g_register_##UniqueID; \

#endif
