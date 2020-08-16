#pragma once

class Graphics;

#include "DXUtils.h"

#include <cstddef>
#include <stdint.h>
#include <vector>

class Buffer {
public:
	Buffer(Graphics& gfx, const void* data, size_t size, D3D11_USAGE usage, uint32_t bindFlags, uint32_t cpuAccessFlags, uint32_t miscFlags, size_t structureSize);
	Buffer(Graphics& gfx, const std::vector<std::byte>& data, D3D11_USAGE usage, uint32_t bindFlags, uint32_t cpuAccessFlags, uint32_t miscFlags, size_t structureSize);
	Buffer(Graphics& gfx, size_t size, D3D11_USAGE usage, uint32_t bindFlags, uint32_t cpuAccessFlags, uint32_t miscFlags, size_t structureSize);
	
	ID3D11Buffer* get() const;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;
};
