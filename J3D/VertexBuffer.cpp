#include "VertexBuffer.h"

#include "Graphics.h"

void VertexBuffer::bind(Graphics& gfx) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	gfx.getContext().IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
}
