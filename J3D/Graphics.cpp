#include "Graphics.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Scene.h"
#include "Camera.h"

#include <array>
#include <iostream>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DXUtils;


Graphics::Graphics(HWND hWnd) : 
	bindableManager(*this),
	pScene(nullptr),
	pCamera(std::make_unique<Camera>()) {

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

	auto pVertexShader = bindableManager.resolve<VertexShader>("./Shaders/VertexShader.cso", "./Shaders/VertexShader.cso");
	auto pPixelShader = bindableManager.resolve<PixelShader>("./Shaders/PixelShader.cso", "./Shaders/PixelShader.cso");

	VertexAttributes vertexAttribs;
	vertexAttribs.positionFormat = Format(AggregateType::VEC3, ComponentType::FLOAT);
	vertexAttribs.normalFormat = Format(AggregateType::VEC3, ComponentType::FLOAT);;
	vertexAttribs.texcoordFormats.push_back(Format(AggregateType::VEC2, ComponentType::FLOAT));
	
	std::vector<float> vertexData{
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	auto pVertexBuffer = bindableManager.resolve<VertexBuffer>("yay", vertexData.data(), vertexData.size() * sizeof(float), vertexAttribs);

	std::vector<uint32_t> indices{
		0, 1, 2
	};

	auto pIndexBuffer = bindableManager.resolve<IndexBuffer>("yay", indices);

	Mesh mesh;
	mesh.addBindable(pVertexBuffer);
	mesh.addBindable(pIndexBuffer);
	mesh.addBindable(pVertexShader);
	mesh.addBindable(pPixelShader);

	bindableManager.get<VSConstantBuffer>("projection")->set(*this, pCamera->getProjection());

	FXMMATRIX model = XMMatrixIdentity();
	mesh.draw(*this, model);
	
	PresentParameters presentParams;
	pSwapChain->Present1(0, 0, &presentParams);
}

ID3D11Device5& Graphics::getDevice() const {
	return **(pDevice.GetAddressOf());
}

ID3D11DeviceContext4& Graphics::getContext() const {
	return **(pContext.GetAddressOf());
}

BindableManager& Graphics::getBindableMgr() {
	return bindableManager;
}

Camera& Graphics::getCamera() {
	return *pCamera;
}

Scene* Graphics::getScene() const {
	return pScene;
}

void Graphics::setScene(Scene* pScene) {
	this->pScene = pScene;
}

void Graphics::windowResized() {
	SwapChainDesc swapChainDesc(pSwapChain);

	pRTV.Reset();
	
	tif(pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, swapChainDesc.Flags));

	ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(pBackBuffer), &pBackBuffer);

	RenderTargetViewDesc rtvDesc;

	pDevice->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &pRTV);

	swapChainDesc = SwapChainDesc(pSwapChain);

	Viewport viewport(swapChainDesc);
	pContext->RSSetViewports(1, &viewport);

	pCamera->resize(swapChainDesc.BufferDesc.Width, swapChainDesc.BufferDesc.Height);
	pCamera->updateProjection();
}


