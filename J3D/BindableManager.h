#pragma once

class Graphics;

#include "Bindable.h"
#include "Util.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <optional>
#include <type_traits>
#include <typeindex>

class BindableManager {
	friend class Graphics;

private:
	BindableManager(Graphics& gfx) : owner(gfx) {};

public:

	template<typename T>
	inline std::optional<std::shared_ptr<T>> get(const std::string& name) {
		static_assert(std::is_base_of<Bindable, T>::value);

		if constexpr (!T::Reusable::value) {
			return {};
		}

		auto it = bindables.find(std::pair(typeid(T), name));

		std::shared_ptr<T> shared;

		if (it != bindables.end() && (shared = std::dynamic_pointer_cast<T>(it->second.lock()))) {
			return shared;
		}

		return {};

	}


	template<typename T, typename... Args>
	inline std::shared_ptr<T> resolve(const std::string& name, Args&&... args) {

		static_assert(std::is_base_of<Bindable, T>::value);

		if constexpr (!T::Reusable::value) {
			return std::make_shared<T>(owner, std::forward<Args>(args)...);
		}

		decltype(bindables)::iterator it = bindables.find({ typeid(T), name });

		std::shared_ptr<T> shared;

		if (it != bindables.end() && (shared = std::static_pointer_cast<T>(it->second.lock()))) {
			return shared;
		}

		shared = std::make_shared<T>(owner, std::forward<Args>(args)...);
		bindables[{ typeid(T), name }] = shared;

		return shared;
	}


private:
	Graphics& owner;

	std::unordered_map<std::pair<std::type_index, std::string>, std::weak_ptr<Bindable>, hash_pair> bindables;

};

