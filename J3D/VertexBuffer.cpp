#include "VertexBuffer.h"

#include "Graphics.h"

#include <cassert>

VertexBuffer::VertexBuffer(Graphics& gfx, const void* data, size_t size, const  DXUtils::VertexAttributes& vertexAttribs) :
	Buffer(gfx, data, size, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0),
	vertexSize(vertexAttribs.getVertexSize()) {}

VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<std::byte>& data, const  DXUtils::VertexAttributes& vertexAttribs) :
	Buffer(gfx, data, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0),
	vertexSize(vertexAttribs.getVertexSize()) {}

void VertexBuffer::bind(Graphics& gfx) {
	UINT stride = static_cast<UINT>(vertexSize);
	UINT offset = 0;

	gfx.getContext().IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
}
