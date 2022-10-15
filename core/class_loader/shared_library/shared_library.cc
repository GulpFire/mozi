#include "shared_library.h"

namespace mozi {
namespace core {

SharedLibrary::SharedLibrary(const char* library_path, int flags)
	: library_path_(library_path)
	, flags_(flags)
	, handle_(nullptr)
{
	Load();
}

SharedLibrary::~LibraryLibrary()
{
	UnLoad();
}

void SharedLibrary::Load()
{
	handle_ = dlopen(library_path_, flags_);
	if (!handle_)
	{
		std::cout << "Failed to load dynamic library." << std::endl;
		return;
	}
	else
	{
		std::cout << "Load " << library_path_ << std::endl;
	}
}

void SharedLibrary::Unload()
{
	if (handle_)
	{
		dlclose(handle_);
		std::cout << "Unload " << library_path_ << std::endl;
	}
	else
	{
		std::cout << "No handle for" << library_path_ << std::endl;
	}
}

void* SharedLibrary::LookUp(const char* sym_name)
{
	void* func = dlsym(handle_, sym_name);
	std::cout << "Look up " << sym_name << " from " << handle_
		<< std::endl;
	if (!func)
	{
		std::cout << "Failed to find symbol " << sym_name << std::endl;
		return nullptr;
	}

	return func;
}

} // namespace core
} // namespace mozi
