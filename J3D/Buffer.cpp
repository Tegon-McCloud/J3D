#include "Buffer.h"
#include "Graphics.h"

Buffer::Buffer(
	Graphics& gfx,
	const void* data,
	size_t size,
	D3D11_USAGE usage,
	uint32_t bindFlags,
	uint32_t cpuAccessFlags,
	uint32_t miscFlags,
	size_t structureSize) {

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = static_cast<UINT>(size);
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.MiscFlags = miscFlags;
	desc.StructureByteStride = static_cast<UINT>(structureSize);

	D3D11_SUBRESOURCE_DATA subDesc;
	subDesc.pSysMem = data;
	subDesc.SysMemPitch = 0;
	subDesc.SysMemSlicePitch = 0;

	tif(gfx.getDevice().CreateBuffer(&desc, &subDesc, &pBuffer));
}

Buffer::Buffer(
	Graphics& gfx,
	const std::vector<std::byte>& data,
	D3D11_USAGE usage,
	uint32_t bindFlags,
	uint32_t cpuAccessFlags,
	uint32_t miscFlags,
	size_t structureSize) :
	Buffer(gfx, data.data(), data.size(), usage, bindFlags, cpuAccessFlags, miscFlags, structureSize) {}

Buffer::Buffer(
	Graphics& gfx,
	size_t size, 
	D3D11_USAGE usage,
	uint32_t bindFlags, 
	uint32_t cpuAccessFlags,
	uint32_t miscFlags,
	size_t structureSize) {
	
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = static_cast<UINT>(size);
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuAccessFlags;
	desc.MiscFlags = miscFlags;
	desc.StructureByteStride = static_cast<UINT>(structureSize);

	tif(gfx.getDevice().CreateBuffer(&desc, nullptr, &pBuffer));

}

ID3D11Buffer* Buffer::get() const {
	return pBuffer.Get();
}
