#include "IndexBuffer.h"

#include "Graphics.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<uint32_t>& indices) :
	Buffer(gfx, indices.data(), indices.size() * sizeof(uint32_t), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0),
	indexCount(indices.size()) {
	
}

void IndexBuffer::bind(Graphics& gfx) {
	

	gfx.getContext().IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

size_t IndexBuffer::count() const {
	return indexCount;
}
