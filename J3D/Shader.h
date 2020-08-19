#pragma once

#include "DXUtils.h"
#include "ConstantBuffer.h"

#include <string>
#include <cassert>
#include <memory>
#include <filesystem>

class ShaderCompiler {
	class Include : public ID3DInclude {
	public:
		Include(const std::filesystem::path& path);

		__declspec(nothrow) HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
		__declspec(nothrow) HRESULT __stdcall Close(LPCVOID pData) override;
	private:
		std::filesystem::path path;
	};

public:
	static Microsoft::WRL::ComPtr<ID3DBlob> fromFile(
		const std::filesystem::path& file,
		std::vector<D3D_SHADER_MACRO> macros,
		const std::string& entryPoint,
		const std::string& target);
};


class VertexShader : public Bindable {
public:
	VertexShader(Graphics& gfx, const std::filesystem::path& file);

	void bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	std::unordered_map<std::string, std::shared_ptr<VSConstantBuffer>> constantBuffers;
};

class PixelShader : public Bindable {
public:
	PixelShader(
		Graphics& gfx,
		const std::filesystem::path& file,
		bool colorMap,
		bool normalMap);

	void bind(Graphics& gfx);

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pShader;
	std::unordered_map<std::string, std::shared_ptr<PSConstantBuffer>> constantBuffers;
};