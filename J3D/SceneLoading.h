#pragma once

#include "DXUtils.h"

#include <fstream>
#include <cstddef>

namespace GLTF {

	struct Buffer {
		size_t size;
		std::ifstream stream;
	};

	struct BufferView {
		Buffer* pBuffer;
		size_t length;
		size_t offset;
		size_t stride;
	};

	struct Accessor {
		BufferView* pView;
		size_t byteOffset;
		size_t count;
		DXUtils::Format format;

		void copyTo(std::byte* pDst, size_t dstOffset, size_t dstStride);

	};

	DXUtils::Format getFormat(const std::string& gltfType, uint64_t gltfComponentType);

}

