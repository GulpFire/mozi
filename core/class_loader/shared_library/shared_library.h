#ifndef MOZI_CORE_CLASS_CLOADER_SHARED_LIBRARY_H
#define MOZI_CORE_CLASS_CLOADER_SHARED_LIBRARY_H

#include <dlfcn.h>
#include <string>
#include <iostream>


namespace mozi {
namespace core {

class SharedLibrary
{
	public:
		explicit SharedLibrary(const char* library_path,
				int flags = RTLD_GLOBAL | RT_LAZY);
		~SharedLibrary();

		void Load();
		void Unload();

		void* LookUp(const char* sym_name);

	private:
		const char* library_path_;
		int flags_;
		void* handle_;
};

} // namespace core
} // namespace mozi

#endif
