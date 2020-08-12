#include "Shader.h"

using Microsoft::WRL::ComPtr;

template<typename T>
inline void reflectConstantBuffers(
	Graphics& gfx,
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pReflection,
	std::unordered_map<std::string, std::shared_ptr<T>>& constantBuffers) {

	D3D11_SHADER_DESC desc;
	tif(pReflection->GetDesc(&desc));

	ID3D11ShaderReflectionConstantBuffer* pBufferReflection;
	D3D11_SHADER_BUFFER_DESC bufferDesc;
	D3D11_SHADER_INPUT_BIND_DESC bindDesc;

	for (uint32_t i = 0; i < desc.ConstantBuffers; i++) {
		pBufferReflection = pReflection->GetConstantBufferByIndex(i);

		tif(pBufferReflection->GetDesc(&bufferDesc));
		tif(pReflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc));

		std::shared_ptr<T> pCBuffer = gfx.getBindableMgr().resolve<T>(bufferDesc.Name, bufferDesc.Size, bindDesc.BindPoint);
		constantBuffers.emplace(bufferDesc.Name, pCBuffer);
	}
}

inline void reflectInputElements(
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pReflection,
	std::vector<D3D11_INPUT_ELEMENT_DESC>& elements) {
	
	D3D11_SHADER_DESC desc;
	tif(pReflection->GetDesc(&desc));

	D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
	D3D11_INPUT_ELEMENT_DESC elementDesc;

	for (uint32_t i = 0; i < desc.InputParameters; i++) {
		pReflection->GetInputParameterDesc(i, &parameterDesc);
		
		elementDesc.SemanticName = parameterDesc.SemanticName;
		elementDesc.SemanticIndex = parameterDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (parameterDesc.Mask == 1) {
			if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		
		} else if (parameterDesc.Mask <= 3) {
			if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;

		} else if (parameterDesc.Mask <= 7) {
			if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		} else if (parameterDesc.Mask <= 15) {
			if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		}

		elements.push_back(elementDesc);
	}

}

VertexShader::VertexShader(Graphics& gfx, const std::string& file) {
	ComPtr<ID3DBlob> pBlob;
	
	std::wstring wfile(file.begin(), file.end());

	tif(D3DReadFileToBlob(wfile.c_str(), &pBlob));
	tif(gfx.getDevice().CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));

	ComPtr<ID3D11ShaderReflection> pReflection;
	tif(D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), __uuidof(pReflection), &pReflection));

	reflectConstantBuffers(gfx, pReflection, constantBuffers);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
	reflectInputElements(pReflection, elements);

	tif(gfx.getDevice().CreateInputLayout(elements.data(), elements.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
}

void VertexShader::bind(Graphics& gfx) {
	gfx.getContext().VSSetShader(pShader.Get(), nullptr, 0);

	gfx.getContext().IASetInputLayout(pInputLayout.Get());

	for (const auto& cb : constantBuffers) {
		cb.second->bind(gfx);
	}
}

PixelShader::PixelShader(Graphics& gfx, const std::string& file) {
	ComPtr<ID3DBlob> pBlob;

	std::wstring wfile(file.begin(), file.end());

	tif(D3DReadFileToBlob(wfile.c_str(), &pBlob));
	tif(gfx.getDevice().CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));

	ComPtr<ID3D11ShaderReflection> pReflection;
	tif(D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), __uuidof(pReflection), &pReflection));

	reflectConstantBuffers(gfx, pReflection, constantBuffers);
}

void PixelShader::bind(Graphics& gfx) {
	gfx.getContext().PSSetShader(pShader.Get(), nullptr, 0);

	for (const auto& cb : constantBuffers) {
		cb.second->bind(gfx);
	}
}
