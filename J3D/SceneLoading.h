#pragma once

#include "DXUtils.h"
#include "Mesh.h"
#include "Scene.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <cstddef>


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
	
	DXUtils::Format getFormat(const std::string& gltfType, uint64_t gltfComponentType);

}

