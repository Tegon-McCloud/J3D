#pragma once

#include "DXUtils.h"
#include "Buffer.h"
#include "Graphics.h"

#include <type_traits>

class ConstantBuffer : public Buffer {
public:
	using Reusable = std::false_type;

public:

	ConstantBuffer(Graphics& gfx, size_t bytes, uint8_t slot) : 
		slot(slot),
		Buffer(gfx, bytes, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, bytes) {}

	//template<typename T>
	//inline ConstantBuffer(Graphics& gfx, uint32_t slot) :
	//	slot(slot),
	//	Buffer(gfx, sizeof(T), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, sizeof(T)) {}

	//template<typename T>
	//inline ConstantBuffer(Graphics& gfx, uint32_t slot, const T& initialData) :
	//	slot(slot),
	//	Buffer(gfx, &initialData, sizeof(T), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, sizeof(T)) {}

	template<typename T>
	void set(Graphics& gfx, const T& data) {

		D3D11_MAPPED_SUBRESOURCE mappedData;

		gfx.getContext().Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy(mappedData.pData, &data, sizeof(T));
		gfx.getContext().Unmap(pBuffer.Get(), 0);
	}

protected:
	uint8_t slot;
};

class VSConstantBuffer : public ConstantBuffer {
	using Buffer::pBuffer;
	using ConstantBuffer::slot;
	using ConstantBuffer::ConstantBuffer;
public:
	void bind(Graphics& gfx) override {
		gfx.getContext().VSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
	}
};
