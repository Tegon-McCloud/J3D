#pragma once

#include <type_traits>

class Bindable {
public:
	using Reusable = std::true_type;

public:
	virtual void bind(class Graphics&) = 0;
};

