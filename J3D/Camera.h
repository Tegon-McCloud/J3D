#pragma once

class Graphics;

#include "DXUtils.h"

class Camera {
public:
	Camera();

	DirectX::XMMATRIX getProjection() const;

	void moveTo(DirectX::FXMVECTOR pos);
	void lookAt(DirectX::FXMVECTOR pos);

	void resize(uint32_t width, uint32_t height);
	void setFov(float fov);
	void setZBounds(float zNear, float zFar);

	void updateProjection();
	void bind(Graphics& gfx);

private:
	DirectX::XMMATRIX projection;

	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 focus;
	DirectX::XMFLOAT4 updir;

	float width, height;
	float fov;
	float zNear, zFar;

}; 

