#pragma once

#include "DXUtils.h"
#include "Buffer.h"
#include "Bindable.h"

#include <vector>
#include <optional>

struct VertexAttributes {
public:
	size_t getVertexSize() const;
	
public:
	DXGI_FORMAT positionFormat;
	std::optional<DXGI_FORMAT> normalFormat, tangentFormat;
	std::vector<DXGI_FORMAT> texcoordFormats, colorFormats;

};

class VertexBuffer : public Bindable, public Buffer {
public:
	VertexBuffer(class Graphics& gfx, const void* data, size_t size, const VertexAttributes& vertexAttribs);
	VertexBuffer(class Graphics& gfx, const std::vector<std::byte>& data, const VertexAttributes& vertexAttribs);

	void bind(class Graphics& gfx) override;

private:
	size_t vertexSize;

};

