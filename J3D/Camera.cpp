#include "Camera.h"

using namespace DirectX;

Camera::Camera() {

    moveTo(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
    lookAt(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));
    updir = { 0.0f, 1.0f, 0.0f, 0.0f };

    updateView();

    setFov(1.6f);
    setZBounds(0.01f, 100.0f);
}

DirectX::XMMATRIX Camera::getView() const {
    return view;
}

DirectX::XMMATRIX Camera::getProjection() const {
    return projection;
}

void Camera::updateView() {
    view = XMMatrixLookAtLH(XMLoadFloat4(&position), XMLoadFloat4(&focus), XMLoadFloat4(&updir));
}

void Camera::updateProjection() {
    projection = XMMatrixPerspectiveFovLH(fov, width/height, zNear, zFar);
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
