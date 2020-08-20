#pragma once

#include "DXUtils.h"
#include "Buffer.h"
#include "Graphics.h"
#include "Bindable.h"

#include <type_traits>

class ConstantBuffer : public Bindable,  public Buffer {
public:

	ConstantBuffer(Graphics& gfx, size_t size, uint8_t slot) : 
		slot(slot),
		Buffer(gfx, size, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0) {}

	template<typename T>
	void set(Graphics& gfx, const T& data) const {

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

class PSConstantBuffer : public ConstantBuffer {
	using Buffer::pBuffer;
	using ConstantBuffer::slot;
	using ConstantBuffer::ConstantBuffer;
public:
	void bind(Graphics& gfx) override {
		gfx.getContext().PSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
	}
};
