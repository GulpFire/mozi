#ifndef MOZI_CORE_CLASS_LOADER_COMPONENt_BASE_H
#define MOZI_CORE_CLASS_LOADER_COMPONENT_BASE_H

class ComponenttBase
{
	public:
		virtual ~ComponentBase() {};
		virtual void Init() = 0;
		virtual void Proc() = 0;
		virtual void Shutdown() = 0;
};

#endif
