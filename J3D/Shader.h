#pragma once

#include "ConstantBuffer.h"

#include <vector>
#include <string>
#include <cassert>
#include <memory>

template<typename CBuffer>
class Shader : public Bindable {
public:

protected:
	inline void reflect(Graphics& gfx, const void* pSrc, size_t srcSize) {

		//static_assert(std::is_base_of<ConstantBuffer, T>::value);

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pShaderReflection;
		tif(D3DReflect(pSrc, srcSize, __uuidof(pShaderReflection), &pShaderReflection));

		D3D11_SHADER_DESC desc;
		tif(pShaderReflection->GetDesc(&desc));

		ID3D11ShaderReflectionConstantBuffer* pBufferReflection;
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;

		for (uint32_t i = 0; i < desc.ConstantBuffers; i++) {
			pBufferReflection = pShaderReflection->GetConstantBufferByIndex(i);

			tif(pBufferReflection->GetDesc(&bufferDesc));
			tif(pShaderReflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc));

			std::shared_ptr<CBuffer> pCBuffer = gfx.getBindableMgr().resolve<CBuffer>(bufferDesc.Name, bufferDesc.Size, bindDesc.BindPoint);
			constantBuffers.emplace(bufferDesc.Name, pCBuffer);
		}

	}

protected:
	std::unordered_map<std::string, std::shared_ptr<CBuffer>> constantBuffers;

};

class VertexShader : public Shader<VSConstantBuffer> {
public:
	VertexShader(Graphics& gfx, const std::string& file);

	void bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pShader;

};

class PixelShader : public Shader<PSConstantBuffer> {
public:
	PixelShader(Graphics& gfx, const std::string& file);

	void bind(Graphics& gfx);

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pShader;
};