#pragma once

#include "DXUtils.h"

#include <fstream>

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

	};

	DXUtils::Format getFormat(const std::string& gltfType, uint64_t gltfComponentType);

}

