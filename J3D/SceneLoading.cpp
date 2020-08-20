#include "SceneLoading.h"

#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <sstream>
#include <filesystem>

using namespace GLTF;

GLTF::Buffer::Buffer() : size(0) {}

GLTF::BufferView::BufferView() : pBuffer(nullptr), length(0), offset(0), stride(0) {}

GLTF::Accessor::Accessor() : pView(nullptr), byteOffset(0), count(0) {}

void GLTF::Accessor::copyTo(std::byte* pDst, size_t dstOffset, size_t dstStride) {

	pView->pBuffer->stream.clear();

	const size_t chunkSize = format.getSize();

	pDst += dstOffset;

	size_t srcOffset = byteOffset + pView->offset;
	const size_t srcStride = pView->stride == 0 ? chunkSize : pView->stride;

	for (uint64_t i = 0; i < count; i++) {
		pView->pBuffer->stream.seekg(srcOffset);

		pView->pBuffer->stream.read(reinterpret_cast<char*>(pDst), chunkSize);

		pDst += dstStride;
		srcOffset += srcStride;
	}
}

DXUtils::Format GLTF::getFormat(const std::string& gltfType, uint64_t gltfComponentType) {
	static const std::unordered_map<std::string, DXUtils::AggregateType> typeMap{
		{ "SCALAR", DXUtils::AggregateType::SCALAR },
		{ "VEC2", DXUtils::AggregateType::VEC2 },
		{ "VEC3", DXUtils::AggregateType::VEC3 },
		{ "VEC4", DXUtils::AggregateType::VEC4 },
		{ "MAT2", DXUtils::AggregateType::MAT2 },
		{ "MAT3", DXUtils::AggregateType::MAT3 },
		{ "MAT4", DXUtils::AggregateType::MAT4 },
	};

	static const std::unordered_map<uint64_t, DXUtils::ComponentType> componentTypeMap{
		{ 5120, DXUtils::ComponentType::BYTE },
		{ 5121, DXUtils::ComponentType::UNSIGNED_BYTE },
		{ 5122, DXUtils::ComponentType::SHORT },
		{ 5123, DXUtils::ComponentType::UNSIGNED_SHORT },
		{ 5125, DXUtils::ComponentType::UNSIGNED_INT },
		{ 5126, DXUtils::ComponentType::FLOAT },
	};

	return DXUtils::Format(typeMap.at(gltfType), componentTypeMap.at(gltfComponentType));
}

D3D11_TEXTURE_ADDRESS_MODE GLTF::getAddressMode(uint64_t gltfAddressMode) {
	switch (gltfAddressMode) {
	case 33071:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case 33648:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case 10497:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	default:
		throw std::runtime_error("Unknown gltf texture address mode: " + gltfAddressMode);
	}
}
