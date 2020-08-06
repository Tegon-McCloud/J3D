#include "Buffer.h"
#include "Graphics.h"

Buffer::Buffer(Graphics& gfx, const void* pData, size_t bytes, uint32_t bindFlags, D3D11_USAGE usage, uint32_t cpuAccessFlags, uint32_t miscFlags, size_t structureSize) {
	DXUtils::BufferDesc desc(
		static_cast<uint32_t>(bytes),
		usage,
		bindFlags,
		cpuAccessFlags,
		miscFlags,
		structureSize
	);
	
	DXUtils::SubresourceData dataDesc(
		pData,
		0,
		0
	);

	gfx.getDevice().CreateBuffer(&desc, &dataDesc, &pBuffer);

}

Buffer::Buffer(Graphics& gfx, size_t bytes, uint32_t bindFlags, D3D11_USAGE usage, uint32_t cpuAccessFlags, uint32_t miscFlags, size_t structureSize) {
	DXUtils::BufferDesc desc(
		static_cast<uint32_t>(bytes),
		usage,
		bindFlags,
		cpuAccessFlags,
		miscFlags,
		structureSize
	);

	gfx.getDevice().CreateBuffer(&desc, nullptr, &pBuffer);
}
