#pragma once

class Scene;
class Camera;

#include "DXUtils.h"
#include "BindableManager.h"

#include <memory>
#include <typeindex>
#include <unordered_map>

class Graphics {
	friend class Window;
public:
	Graphics(HWND hWnd);
	~Graphics();

	void render();

	ID3D11Device5& getDevice() const;
	ID3D11DeviceContext4& getContext() const;
	Camera& getCamera();

	Scene* getScene() const;
	void setScene(Scene* pScene);


	template<typename T>
	ResourceManager<T>& getResourceMgr() {
		ResourceManager<T>* pMgr;
		auto it = resourceManagers.find(typeid(T));

		if (it == resourceManagers.end()) {
			std::unique_ptr<ResourceManager<T>> unique = std::make_unique<ResourceManager<T>>(*this);
			pMgr = unique.get();
			resourceManagers[typeid(T)] = std::move(unique);
		} else {
			assert(dynamic_cast<ResourceManager<T>*>(it->second.get()));
			pMgr = static_cast<ResourceManager<T>*>(it->second.get());
		}

		return *pMgr;
	}


private:
	void windowResized();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device5> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4> pContext;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	std::unordered_map<std::type_index, std::unique_ptr<ResourceManagerBase>> resourceManagers;

	std::unique_ptr<Camera> pCamera;
	Scene* pScene;

};

