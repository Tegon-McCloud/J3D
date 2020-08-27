#include "Camera.h"
#include "Graphics.h"
#include "ConstantBuffer.h"

using namespace DirectX;

Camera::Camera() {

    moveTo(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
    lookAt(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));
    updir = { 0.0f, 1.0f, 0.0f, 0.0f };

    setFov(1.6f);
    setZBounds(0.01f, 100.0f);
}

DirectX::XMMATRIX Camera::getProjection() const {
    return projection;
}

void Camera::updateProjection() {
    projection = XMMatrixPerspectiveFovLH(fov, width/height, zNear, zFar);
}

void Camera::bind(Graphics& gfx) {
    XMMATRIX view = XMMatrixLookAtLH(
        XMLoadFloat4(&position),
        XMLoadFloat4(&focus),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );

    auto pViewProjectionCBuffer = gfx.getResourceMgr<VSConstantBuffer>().get("viewProjection");

    if (pViewProjectionCBuffer) {
        pViewProjectionCBuffer->set(gfx,
            XMMatrixMultiplyTranspose(
                view,
                projection
            )
        );
    }

    auto pCameraDataCBuffer = gfx.getResourceMgr<PSConstantBuffer>().get("cameraData");

    if (pCameraDataCBuffer) {
        pCameraDataCBuffer->set(gfx,
            position
        );
    }


}

void Camera::moveTo(DirectX::FXMVECTOR pos) {
    XMStoreFloat4(&position, pos);
}

void Camera::lookAt(DirectX::FXMVECTOR pos) {
    XMStoreFloat4(&focus, pos);
}

void Camera::resize(uint32_t width, uint32_t height) {
    this->width = static_cast<float>(width);
    this->height = static_cast<float>(height);
}

void Camera::setFov(float fov) {
    this->fov = fov;
}

void Camera::setZBounds(float zNear, float zFar) {
    this->zNear = zNear;
    this->zFar = zFar;
}
