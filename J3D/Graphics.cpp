#include "Graphics.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Scene.h"

#include <array>
#include <iostream>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DXUtils;


Graphics::Graphics(HWND hWnd) : 
	bindableManager(*this),
	pScene(std::make_unique<Scene>()) {

	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11DeviceContext> pContext;

	UINT deviceFlags = 0;
#ifdef _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_FEATURE_LEVEL lvl = D3D_FEATURE_LEVEL_11_1;

	SwapChainDesc swapChainDesc(hWnd);

	tif(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceFlags,
		&lvl,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	));

	tif(pDevice.As(&this->pDevice));
	tif(pSwapChain.As(&this->pSwapChain));
	tif(pContext.As(&this->pContext));
	
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	std::wcout << NPath << std::endl;
}

Graphics::~Graphics() {
#ifdef _DEBUG
	ComPtr<ID3D11Debug> pDebug;
	tif(pDevice.As(&pDebug));
#endif

	pRTV.Reset();
	pDSV.Reset();
	pContext.Reset();
	pSwapChain.Reset();
	pDevice.Reset();


#ifdef _DEBUG
	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
}

void Graphics::render() {
	
	const float rgba[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	pContext->ClearRenderTargetView(pRTV.Get(), rgba);

	pContext->OMSetRenderTargets(1, pRTV.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ComPtr<ID3DBlob> pShaderBlob;
	
	tif(D3DReadFileToBlob(L"./Shaders/VertexShader.cso", &pShaderBlob));
	
	std::shared_ptr<VertexShader> pVertexShader = bindableManager.resolve<VertexShader>("./Shaders/VertexShader.cso", "./Shaders/VertexShader.cso");

	ComPtr<ID3D11InputLayout> pInputLayout;

	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	pDevice->CreateInputLayout(inputElementDescs, static_cast<uint32_t>(std::size(inputElementDescs)), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &pInputLayout);

	pContext->IASetInputLayout(pInputLayout.Get());

	tif(D3DReadFileToBlob(L"./Shaders/PixelShader.cso", &pShaderBlob));
	ComPtr<ID3D11PixelShader> pPixelShader;
	tif(pDevice->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, &pPixelShader));
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	std::array<Vertex, 3> vertices{
		Vertex{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		Vertex{ { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		Vertex{ { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	};

	auto pVertexBuffer = bindableManager.resolve<VertexBuffer>("yay", vertices);

	std::array<Index, 3> indices{
		0, 1, 2
	};

	auto pIndexBuffer = bindableManager.resolve<IndexBuffer>("yay", indices);

	FXMMATRIX mvp = XMMatrixTranslation(0.5f, 0.0f, 0.0f);
	
	Mesh mesh;
	mesh.addBindable(pVertexBuffer);
	mesh.addBindable(pIndexBuffer);
	mesh.addBindable(pVertexShader);

	mesh.draw(*this, mvp);
	
	PresentParameters presentParams;
	pSwapChain->Present1(0, 0, &presentParams);
}

ID3D11Device5& Graphics::getDevice() const {
	return **(pDevice.GetAddressOf());
}

ID3D11DeviceContext4& Graphics::getContext() const {
	return **(pContext.GetAddressOf());
}

Scene* Graphics::getScene() {
	return pScene.get();
}

void Graphics::windowResized() {
	SwapChainDesc swapChainDesc(pSwapChain);

	pRTV.Reset();
	
	tif(pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, swapChainDesc.Flags));

	ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(pBackBuffer), &pBackBuffer);

	RenderTargetViewDesc rtvDesc;

	pDevice->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &pRTV);

	Viewport viewport(swapChainDesc);
	pContext->RSSetViewports(1, &viewport);

	swapChainDesc = SwapChainDesc(pSwapChain);
	std::cout << swapChainDesc.BufferDesc.Width << ", " << swapChainDesc.BufferDesc.Height << "\n";
}


