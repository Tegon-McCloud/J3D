#include "Shader.h"

using Microsoft::WRL::ComPtr;

VertexShader::VertexShader(Graphics& gfx, const std::string& file) {
	ComPtr<ID3DBlob> pBlob;
	
	std::wstring wfile(file.begin(), file.end());

	tif(D3DReadFileToBlob(wfile.c_str(), &pBlob));
	tif(gfx.getDevice().CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));

	reflect(gfx, pBlob->GetBufferPointer(), pBlob->GetBufferSize());
}

void VertexShader::bind(Graphics& gfx) {
	gfx.getContext().VSSetShader(pShader.Get(), nullptr, 0);

	for (const auto& cb : constantBuffers) {
		cb.second->bind(gfx);
	}
}

PixelShader::PixelShader(Graphics& gfx, const std::string& file) {
	ComPtr<ID3DBlob> pBlob;

	std::wstring wfile(file.begin(), file.end());

	tif(D3DReadFileToBlob(wfile.c_str(), &pBlob));
	tif(gfx.getDevice().CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));

	reflect(gfx, pBlob->GetBufferPointer(), pBlob->GetBufferSize());
}

void PixelShader::bind(Graphics& gfx) {
	gfx.getContext().PSSetShader(pShader.Get(), nullptr, 0);

	for (const auto& cb : constantBuffers) {
		cb.second->bind(gfx);
	}
}
