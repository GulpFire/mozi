#ifndef MOZI_CORE_CLASS_LOADER_CLASS_LOADER_H
#define MOZI_CORE_CLASS_LOADER_CLASS_LOADER_H

#include "shared_library/shared_library.h"

#include <map>
#include <memory>
#include <string>

namespace mozi {
namespace core {

using SharedLibraryPtr = std::shared_ptr<SharedLibrary>;
using SharedLibraryMap = std::map<std::string, SharedLibraryPtr> SharedLibraryMap;

class ClassLoader
{
	public:
		void Load();
		void UnLoad();

		bool isLoaded () const;

	private:
	static SharedLibraryMap library_map_;
};

} // namespace core
} // namespace mozi

#endif
