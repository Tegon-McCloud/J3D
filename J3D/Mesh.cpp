#include "Mesh.h"
#include "Shader.h"

#include "Graphics.h" 

using namespace DirectX;

void Mesh::draw(Graphics& gfx, DirectX::FXMMATRIX transform) {
	for (const auto& pBindable : bindables) {
		pBindable->bind(gfx);
	}
	pIndexBuffer->bind(gfx);
	
	pModelViewTransformBuffer->set(gfx, XMMatrixTranspose(transform));

	gfx.getContext().DrawIndexed(pIndexBuffer->count(), 0, 0);
}

void Mesh::addBindable(std::shared_ptr<Bindable> pBindable) {

	if (std::dynamic_pointer_cast<IndexBuffer>(pBindable)) {
		pIndexBuffer = std::static_pointer_cast<IndexBuffer>(pBindable);
	} else {
		bindables.push_back(pBindable);
	}

	if (std::dynamic_pointer_cast<VertexShader>(pBindable)) {
		pModelViewTransformBuffer = std::static_pointer_cast<VertexShader>(pBindable)->getCBuffer("modelView");
	}
}
