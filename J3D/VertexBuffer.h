#pragma once

#include "DXUtils.h"
#include "Buffer.h"

#include <vector>

struct Vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texcoord;
};

class VertexBuffer : public Buffer {
public:
	template<typename Container>
	inline VertexBuffer(class Graphics& gfx, const Container& vertices) : Buffer(gfx, vertices, D3D11_BIND_VERTEX_BUFFER) {}


	void bind(class Graphics& gfx) override;

private:

};

