#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Graphics.h" 

using namespace DirectX;

void Mesh::draw(Graphics& gfx, DirectX::FXMMATRIX transform) {
	for (const auto& pBindable : bindables) {
		pBindable->bind(gfx);
	}
	pIndexBuffer->bind(gfx);
	
	gfx.getBindableMgr().get<VSConstantBuffer>("modelView")->set(gfx, XMMatrixMultiplyTranspose(transform, gfx.getCamera().getView()));

	gfx.getContext().DrawIndexed(pIndexBuffer->count(), 0, 0);
}

void Mesh::addBindable(std::shared_ptr<Bindable> pBindable) {

	if (std::dynamic_pointer_cast<IndexBuffer>(pBindable)) {
		pIndexBuffer = std::static_pointer_cast<IndexBuffer>(pBindable);
	} else {
		bindables.push_back(pBindable);
	}
}
