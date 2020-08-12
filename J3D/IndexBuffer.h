#pragma once

#include "DXUtils.h"
#include "Buffer.h"
#include "Bindable.h"

#include <type_traits>
#include <vector>

class IndexBuffer : public Bindable, public Buffer {
public:
	IndexBuffer(class Graphics& gfx, const std::vector<uint32_t>& indices);


	void bind(class Graphics& gfx) override;

	size_t count() const;

private:
	size_t indexCount;

};

