#pragma once

#include "DXUtils.h"
#include "BindableManager.h"

class Graphics {
	friend class Window;
public:
	Graphics(HWND hWnd);
	~Graphics();


	void render();

	ID3D11Device5& getDevice() const;
	ID3D11DeviceContext4& getContext() const;

private:
	void windowResized();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device5> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4> pContext;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	BindableManager bindableManager;

};

