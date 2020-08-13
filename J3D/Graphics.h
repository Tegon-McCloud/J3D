#pragma once

class Scene;
class Camera;

#include "DXUtils.h"
#include "BindableManager.h"

#include <memory>

class Graphics {
	friend class Window;
public:
	Graphics(HWND hWnd);
	~Graphics();

	void render();

	ID3D11Device5& getDevice() const;
	ID3D11DeviceContext4& getContext() const;
	BindableManager& getBindableMgr();
	Camera& getCamera();

	Scene* getScene() const;
	void setScene(Scene* pScene);

private:
	void windowResized();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device5> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4> pContext;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	BindableManager bindableManager;

	std::unique_ptr<Camera> pCamera;
	Scene* pScene;

};

