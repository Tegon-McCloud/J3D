#pragma once

#include "DXUtils.h"
#include "Buffer.h"
#include "Bindable.h"

#include <vector>
#include <optional>

struct VertexAttributes {
public:
	VertexAttributes() = default;

	size_t getVertexSize() const;
	
	size_t positionOffset() const;
	size_t normalOffset() const;
	size_t tangentOffset() const;
	size_t texcoordOffset(size_t i) const;
	size_t colorOffset(size_t i) const;


public:
	DXUtils::Format positionFormat, normalFormat, tangentFormat;
	std::vector <DXUtils::Format> texcoordFormats, colorFormats;

};

class VertexBuffer : public Bindable, public Buffer {
public:
	VertexBuffer(class Graphics& gfx, const void* data, size_t size, const VertexAttributes& vertexAttribs);
	VertexBuffer(class Graphics& gfx, const std::vector<std::byte>& data, const VertexAttributes& vertexAttribs);

	void bind(class Graphics& gfx) override;

private:
	size_t vertexSize;

};

