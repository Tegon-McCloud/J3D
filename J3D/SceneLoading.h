#pragma once

#include "DXUtils.h"

#include <fstream>

namespace SceneLoading {

	struct Buffer {
		size_t size;
		std::ifstream data;
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
		DXUtils::Format type;

	};

}

