#include "Graphics.h"

#include "ConstantBuffer.h"
#include "Scene.h"
#include "Camera.h"
#include "Buffer.h"
#include "Sampler.h"
#include "Lighting.h"

#include "Texture.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DXUtils;


Graphics::Graphics(HWND hWnd) : 
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

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;

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
	
	const float rgba[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	pContext->ClearRenderTargetView(pRTV.Get(), rgba);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pContext->OMSetRenderTargets(1, pRTV.GetAddressOf(), pDSV.Get());

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	auto pProjectionCBuffer = getResourceMgr<VSConstantBuffer>().get("projection"); 
	if (pProjectionCBuffer) {
		pProjectionCBuffer->set(*this, XMMatrixTranspose(pCamera->getProjection()));
	}

	pCamera->moveTo(XMVectorSet(0.0f, 4.0f, -4.0f, 1.0f));
	pCamera->lookAt(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
	pCamera->updateView();
	
	//static auto tex = getResourceMgr<PSTexture2D>().resolve("yay", std::filesystem::path("./Models/anvil/anvil_normal.png"));
	//tex->bind(*this, 0);

	//static auto pSampler = getResourceMgr<PSSampler>().resolve("yay", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
	//pSampler->bind(*this, 0);

	DirectionalLight light;
	light.color = { 1.0f, 1.0f, 1.0 };
	DirectX::XMStoreFloat3(&light.direction, XMVector3Normalize(
		XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f)));

	ComPtr<ID3D11Buffer> lightBuffer;
	ComPtr<ID3D11ShaderResourceView> lightView;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(DirectionalLight);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(DirectionalLight);

	D3D11_SUBRESOURCE_DATA dataDesc;
	dataDesc.pSysMem = &light;
	dataDesc.SysMemPitch = 0;
	dataDesc.SysMemSlicePitch = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = DXGI_FORMAT_UNKNOWN;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	viewDesc.Buffer.ElementOffset = 0;
	viewDesc.Buffer.NumElements = 1;

	tif(pDevice->CreateBuffer(&desc, &dataDesc, &lightBuffer));
	tif(pDevice->CreateShaderResourceView(lightBuffer.Get(), &viewDesc, &lightView));

	pContext->PSSetShaderResources(0, 1, lightView.GetAddressOf());

	if (pScene) {
		pScene->draw(*this);
	}

	DXGI_PRESENT_PARAMETERS presentParams;
	presentParams.DirtyRectsCount = 0;
	presentParams.pDirtyRects = nullptr;
	presentParams.pScrollOffset = nullptr;
	presentParams.pScrollRect = nullptr;

	pSwapChain->Present1(0, 0, &presentParams);
}

ID3D11Device5& Graphics::getDevice() const {
	return **(pDevice.GetAddressOf());
}

ID3D11DeviceContext4& Graphics::getContext() const {
	return **(pContext.GetAddressOf());
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
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	tif(pSwapChain->GetDesc(&swapChainDesc));

	pRTV.Reset();
	pDSV.Reset();

	tif(pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, swapChainDesc.Flags));

	// rebuild rtv
	ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(pBackBuffer), &pBackBuffer);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	tif(pDevice->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &pRTV));

	tif(pSwapChain->GetDesc(&swapChainDesc));

	// rebuild dsv
	ComPtr<ID3D11Texture2D> dstex;

	D3D11_TEXTURE2D_DESC dstexDesc;
	dstexDesc.Width = swapChainDesc.BufferDesc.Width;
	dstexDesc.Height = swapChainDesc.BufferDesc.Height;
	dstexDesc.MipLevels = 1;
	dstexDesc.ArraySize = 1;
	dstexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dstexDesc.SampleDesc.Count = 1;
	dstexDesc.SampleDesc.Quality = 0;
	dstexDesc.Usage = D3D11_USAGE_DEFAULT;
	dstexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dstexDesc.CPUAccessFlags = 0;
	dstexDesc.MiscFlags = 0;
	
	pDevice->CreateTexture2D(&dstexDesc, nullptr, &dstex);

	//D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	//dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//dsvDesc.Texture2D
	
	pDevice->CreateDepthStencilView(dstex.Get(), nullptr, &pDSV);

	// bind new viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(swapChainDesc.BufferDesc.Width);
	viewport.Height = static_cast<float>(swapChainDesc.BufferDesc.Height);
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.MaxDepth = D3D11_MAX_DEPTH;

	pContext->RSSetViewports(1, &viewport);

	pCamera->resize(swapChainDesc.BufferDesc.Width, swapChainDesc.BufferDesc.Height);
	pCamera->updateProjection();
}


