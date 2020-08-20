#pragma once

class Graphics;

#include "Util.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <typeindex>

class ResourceManagerBase {
public:
	ResourceManagerBase(Graphics& gfx) : owner(gfx) {};
	ResourceManagerBase(const ResourceManagerBase&) = delete;
	ResourceManagerBase& operator=(const ResourceManagerBase&) = delete;


private:
	// allow for dynamic_cast
	virtual void makeTypePolymophic() {};

protected:
	
	Graphics& owner;
};

template<typename Resource>
class ResourceManager : public ResourceManagerBase {
public:
	ResourceManager(Graphics& gfx) : ResourceManagerBase(gfx) {}
	
	std::shared_ptr<Resource> get(const std::string& name) {
		auto it = resourceMap.find(name);

		std::shared_ptr<Resource> shared;

		if (it != resourceMap.end() && (shared = it->second.lock())) {
			return shared;
		}

		return nullptr;
	}

	template<typename... Args>
	std::shared_ptr<Resource> resolve(const std::string& name, Args&&... args) {

		if constexpr (!Resource::Reusable::value) {
			return std::make_shared<Resource>(owner, std::forward<Args>(args)...);
		}

		if (name == "") {
			return std::make_shared<Resource>(owner, std::forward<Args>(args)...);
		}

		auto it = resourceMap.find(name);

		std::shared_ptr<Resource> shared;

		if (it != resourceMap.end() && (shared = it->second.lock())) {
			return shared;
		}

		shared = std::make_shared<Resource>(owner, std::forward<Args>(args)...);
		resourceMap[name] = shared;

		return shared;
	}

private:
	std::unordered_map<std::string, std::weak_ptr<Resource>> resourceMap;
};
