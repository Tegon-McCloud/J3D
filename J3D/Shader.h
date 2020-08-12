#pragma once

#include "DXUtils.h"
#include "ConstantBuffer.h"

#include <string>
#include <cassert>
#include <memory>

class VertexShader : public Bindable {
public:
	VertexShader(Graphics& gfx, const std::string& file);

	void bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	std::unordered_map<std::string, std::shared_ptr<VSConstantBuffer>> constantBuffers;
};

class PixelShader : public Bindable {
public:
	PixelShader(Graphics& gfx, const std::string& file);

	void bind(Graphics& gfx);

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pShader;
	std::unordered_map<std::string, std::shared_ptr<PSConstantBuffer>> constantBuffers;
};