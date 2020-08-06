#include "IndexBuffer.h"

#include "Graphics.h"

void IndexBuffer::bind(Graphics& gfx) {
	

	gfx.getContext().IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

size_t IndexBuffer::count() {
	return indexCount;
}
