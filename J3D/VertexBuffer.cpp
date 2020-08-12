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

size_t formatSize(DXGI_FORMAT format) {
	switch (format) {
	case DXGI_FORMAT_R32_FLOAT:
		return 4;
	case DXGI_FORMAT_R32G32_FLOAT:
		return 8;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return 12;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return 16;
	default:
		assert("format not supported for vertex attributes" && false);
	}
}

size_t VertexAttributes::getVertexSize() const {
	size_t size = formatSize(positionFormat);
	
	if (normalFormat.has_value()) {
		size += formatSize(normalFormat.value());
	}
	
	if (tangentFormat.has_value()) {
		size += formatSize(tangentFormat.value());
	}

	for (DXGI_FORMAT format : texcoordFormats) {
		size += formatSize(format);
	}

	for (DXGI_FORMAT format : colorFormats) {
		size += formatSize(format);
	}

	return size;
}
