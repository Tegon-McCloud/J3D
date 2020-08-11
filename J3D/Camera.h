#pragma once

#include "DXUtils.h"

class Camera {
	friend class Graphics;
public:
	Camera();

	DirectX::XMMATRIX getView() const;
	DirectX::XMMATRIX getProjection() const;

	void moveTo(DirectX::FXMVECTOR pos);
	void lookAt(DirectX::FXMVECTOR pos);

	void resize(uint32_t width, uint32_t height);
	void setFov(float fov);
	void setZBounds(float zNear, float zFar);

private:
	void updateView();
	void updateProjection();

private:

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 focus;
	DirectX::XMFLOAT4 updir;

	float width, height;
	float fov;
	float zNear, zFar;

}; 

