#include "Mesh.h"

#include "Graphics.h" 

void Mesh::draw(class Graphics& gfx) {
	for (const auto& pBindable : bindables) {
		pBindable->bind(gfx);
	}
	pIndexBuffer->bind(gfx);

	gfx.getContext().DrawIndexed(pIndexBuffer->count(), 0, 0);
}

void Mesh::addBindable(std::shared_ptr<Bindable> pBindable) {

	if (std::dynamic_pointer_cast<IndexBuffer>(pBindable)) {
		pIndexBuffer = std::static_pointer_cast<IndexBuffer>(pBindable);
	} else {
		bindables.push_back(pBindable);
	}
}
