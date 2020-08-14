#pragma once

#include "DXUtils.h"
#include "Buffer.h"
#include "Bindable.h"

#include <vector>
#include <optional>

class VertexBuffer : public Bindable, public Buffer {
public:
	VertexBuffer(class Graphics& gfx, const void* data, size_t size, const DXUtils::VertexAttributes& vertexAttribs);
	VertexBuffer(class Graphics& gfx, const std::vector<std::byte>& data, const DXUtils::VertexAttributes& vertexAttribs);

	void bind(class Graphics& gfx) override;

private:
	size_t vertexSize;

};

