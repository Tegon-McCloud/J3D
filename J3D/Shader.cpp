#include "Shader.h"

using Microsoft::WRL::ComPtr;

VertexShader::VertexShader(Graphics& gfx) {

	ComPtr<ID3DBlob> pBlob;

	tif(D3DReadFileToBlob(L"./Shaders/VertexShader.cso", &pBlob));
	tif(gfx.getDevice().CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));

	reflect<VSConstantBuffer>(gfx, pBlob->GetBufferPointer(), pBlob->GetBufferSize());
}

void VertexShader::bind(Graphics& gfx) {
	gfx.getContext().VSSetShader(pShader.Get(), nullptr, 0);


	for (const auto& cb : constantBuffers) {
		cb.second->bind(gfx);
	}
}
