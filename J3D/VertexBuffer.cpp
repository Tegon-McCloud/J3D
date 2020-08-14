#include "VertexBuffer.h"

#include "Graphics.h"

#include <cassert>

VertexBuffer::VertexBuffer(Graphics& gfx, const void* data, size_t size, const VertexAttributes& vertexAttribs) :
	Buffer(gfx, data, size, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0),
	vertexSize(vertexAttribs.getVertexSize()) {}

VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<std::byte>& data, const VertexAttributes& vertexAttribs) :
	Buffer(gfx, data, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0),
	vertexSize(vertexAttribs.getVertexSize()) {}

void VertexBuffer::bind(Graphics& gfx) {
	UINT stride = static_cast<UINT>(vertexSize);
	UINT offset = 0;

	gfx.getContext().IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
}

size_t VertexAttributes::getVertexSize() const {
	size_t size = positionFormat.getSize();
	size += normalFormat.getSize();
	size += tangentFormat.getSize();
	
	for (DXUtils::Format format : texcoordFormats) {
		size += format.getSize();
	}

	for (DXUtils::Format format : colorFormats) {
		size += format.getSize();
	}

	return size;
}

size_t VertexAttributes::positionOffset() const {
	return 0;
}

size_t VertexAttributes::normalOffset() const {
	return positionFormat.getSize();
}

size_t VertexAttributes::tangentOffset() const {
	return normalOffset() + normalFormat.getSize();
}

size_t VertexAttributes::texcoordOffset(size_t i) const {
	
	if (i == 0) {
		return tangentOffset() + tangentFormat.getSize();
	}
	return texcoordOffset(i - 1) + texcoordFormats[i - 1].getSize();
}

size_t VertexAttributes::colorOffset(size_t i) const {
	if (i == 0) {
		return texcoordOffset(texcoordFormats.size() - 1);
	}
	return colorOffset(i - 1) + colorFormats[i - 1].getSize();
}
