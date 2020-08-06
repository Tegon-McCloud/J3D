#pragma once

#include "DXUtils.h"
#include "Buffer.h"
#include <type_traits>

typedef uint32_t Index;

class IndexBuffer : public Buffer {
public:
	template<typename Container>
	inline IndexBuffer(class Graphics& gfx, const Container& indices) : Buffer(gfx, indices, D3D11_BIND_INDEX_BUFFER) {
		static_assert(std::is_same<Container::value_type, Index>::value);

		indexCount = indices.size();
	}

	void bind(class Graphics& gfx) override;

	size_t count();

private:
	size_t indexCount;

};

