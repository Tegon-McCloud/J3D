#pragma once

#include "DXUtils.h"
#include "Bindable.h"

#include <iostream>

class Buffer : public Bindable {
protected:
	
	Buffer(class Graphics& gfx, const void* pData, size_t bytes, uint32_t bindFlags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccessFlags = 0, uint32_t miscFlags = 0, size_t structureSize = 0);
	template<typename T>
	inline Buffer(class Graphics& gfx, const T* pData, size_t count, uint32_t bindFlags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccessFlags = 0, uint32_t miscFlags = 0) :
		Buffer(gfx, pData, sizeof(T)* count, bindFlags, usage, cpuAccessFlags, miscFlags, sizeof(T)) {
		std::cout << "Template constructor called";
	}
	template<typename Container>
	inline Buffer(class Graphics& gfx, const Container& container, uint32_t bindFlags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccessFlags = 0, uint32_t miscFlags = 0) :
		Buffer(gfx, container.data(), container.size() * sizeof(Container::value_type), bindFlags, usage, cpuAccessFlags, miscFlags, sizeof(Container::value_type)) {}

	Buffer(class Graphics& gfx, size_t bytes, uint32_t bindFlags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccessFlags = 0, uint32_t miscFlags = 0, size_t structureSize = 0);

	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
};
