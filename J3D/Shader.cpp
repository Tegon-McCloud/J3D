#include "Shader.h"

#include <vector>
#include <fstream>
#include <iostream>

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

//VertexShader::VertexShader(Graphics& gfx, const std::string& file) {
//	ComPtr<ID3DBlob> pBlob;
//	
//	std::wstring wfile(file.begin(), file.end());
//
//	tif(D3DReadFileToBlob(wfile.c_str(), &pBlob));
//	tif(gfx.getDevice().CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));
//
//	ComPtr<ID3D11ShaderReflection> pReflection;
//	tif(D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), __uuidof(pReflection), &pReflection));
//
//	reflectConstantBuffers(gfx, pReflection, constantBuffers);
//
//	std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
//	reflectInputElements(pReflection, elements);
//
//	tif(gfx.getDevice().CreateInputLayout(elements.data(), static_cast<UINT>(elements.size()), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
//}

VertexShader::VertexShader(Graphics& gfx, const std::filesystem::path& file) {

	std::vector<D3D_SHADER_MACRO> macros;

	ComPtr<ID3DBlob> pBlob = ShaderCompiler::fromFile(file, macros, "main", "vs_5_0");
	tif(gfx.getDevice().CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));

	ComPtr<ID3D11ShaderReflection> pReflection;
	tif(D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), __uuidof(pReflection), &pReflection));

	reflectConstantBuffers(gfx, pReflection, constantBuffers);

	std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
	reflectInputElements(pReflection, elements);

	tif(gfx.getDevice().CreateInputLayout(elements.data(), static_cast<UINT>(elements.size()), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));

}

void VertexShader::bind(Graphics& gfx) {
	gfx.getContext().VSSetShader(pShader.Get(), nullptr, 0);

	gfx.getContext().IASetInputLayout(pInputLayout.Get());

	for (const auto& cb : constantBuffers) {
		cb.second->bind(gfx);
	}
}

//PixelShader::PixelShader(Graphics& gfx, const std::string& file) {
//	ComPtr<ID3DBlob> pBlob;
//
//	std::wstring wfile(file.begin(), file.end());
//
//	tif(D3DReadFileToBlob(wfile.c_str(), &pBlob));
//	tif(gfx.getDevice().CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader));
//
//	ComPtr<ID3D11ShaderReflection> pReflection;
//	tif(D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), __uuidof(pReflection), &pReflection));
//
//	reflectConstantBuffers(gfx, pReflection, constantBuffers);
//}

PixelShader::PixelShader(Graphics& gfx, const std::filesystem::path& file, bool colorMap, bool normalMap) {

	std::vector<D3D_SHADER_MACRO> macros;
	
	if (colorMap) macros.push_back({ "COLOR_MAP", nullptr });
	if (normalMap) macros.push_back({ "NORMAL_MAP", nullptr });

	ComPtr<ID3DBlob> pBlob = ShaderCompiler::fromFile(file, macros, "main", "ps_5_0");

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

ShaderCompiler::Include::Include(const std::filesystem::path& path) : path(path) {}

HRESULT __stdcall ShaderCompiler::Include::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) {
	if (IncludeType == D3D_INCLUDE_SYSTEM) {
		return E_FAIL;
	}

	try {
		std::filesystem::path file = path / pFileName;

		std::ifstream ifs(file, std::ios::binary | std::ios::ate);
		if (ifs.is_open()) {
			size_t fileSize = ifs.tellg();
			char* buffer = new char[fileSize];
			ifs.seekg(0, std::ios::beg);
			ifs.read(buffer, fileSize);
			ifs.close();

			*ppData = buffer;
			*pBytes = fileSize;
		} else {
			return E_FAIL;
		}

		return S_OK;
	} catch (std::exception& e) {
		return E_FAIL;
	}
}

HRESULT __stdcall ShaderCompiler::Include::Close(LPCVOID pData) {
	const char* pBuffer = reinterpret_cast<const char*>(pData);
	delete[] pBuffer;
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCompiler::fromFile(const std::filesystem::path& file, std::vector<D3D_SHADER_MACRO> macros, const std::string& entryPoint, const std::string& target) {
	ComPtr<ID3DBlob> pBlob;
	ComPtr<ID3DBlob> pErrorBlob;

	macros.push_back({ nullptr, nullptr });

	Include include(file.parent_path());

	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#elif
	flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	if (D3DCompileFromFile(file.c_str(), macros.data(), &include, entryPoint.c_str(), target.c_str(), flags, 0, &pBlob, &pErrorBlob) != S_OK) {
		if (pErrorBlob) {
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
		}
		throw std::runtime_error("Failed to compile pixel shader");
	}

	return pBlob;

}
