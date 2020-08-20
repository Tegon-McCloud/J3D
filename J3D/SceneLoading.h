#pragma once

#include "DXUtils.h"
#include "Mesh.h"
#include "Scene.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <cstddef>
#include <string>


namespace GLTF {

	struct Buffer {
		Buffer();

		size_t size;
		std::ifstream stream;
	};

	struct BufferView {
		BufferView();

		Buffer* pBuffer;
		size_t length;
		size_t offset;
		size_t stride;
	};

	struct Accessor {
		Accessor();

		BufferView* pView;
		size_t byteOffset;
		size_t count;
		DXUtils::Format format;

		void copyTo(std::byte* pDst, size_t dstOffset, size_t dstStride);
	};

	struct Image {
		std::filesystem::path file;
		std::string name;
	};

	struct Sampler {
		D3D11_TEXTURE_ADDRESS_MODE uMode, vMode;
	};

	struct Texture {
		Image* pImage;
		Sampler* pSampler;
	};
	
	DXUtils::Format getFormat(const std::string& gltfType, uint64_t gltfComponentType);
	D3D11_TEXTURE_ADDRESS_MODE getAddressMode(uint64_t gltfAddressMode);

}

