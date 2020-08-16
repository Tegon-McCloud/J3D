#pragma once

#include "Win32Utils.h"

#include <wrl.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <stdint.h>
#include <vector>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace DXUtils {

	class AggregateType {
	public:
		static const AggregateType SCALAR;
		static const AggregateType VEC2;
		static const AggregateType VEC3;
		static const AggregateType VEC4;
		static const AggregateType MAT2;
		static const AggregateType MAT3;
		static const AggregateType MAT4;

	public:
		AggregateType();
	private:
		AggregateType(size_t count);
	public:
		size_t getCount() const;
	private:
		size_t count;
	};

	struct ComponentType {
	public:
		static const ComponentType BYTE;
		static const ComponentType UNSIGNED_BYTE;
		static const ComponentType SHORT;
		static const ComponentType UNSIGNED_SHORT;
		static const ComponentType UNSIGNED_INT;
		static const ComponentType FLOAT;
	public:
		ComponentType();
	private:
		ComponentType(size_t bytes);
	public:
		size_t getBytes() const;
	private:
		size_t bytes;

	};

	struct Format {
		Format() = default;
		Format(AggregateType aggregateType, ComponentType componentType);

		AggregateType aggregateType;
		ComponentType componentType;

		size_t getSize() const;
	};

	struct VertexAttributes {
	public:
		VertexAttributes() = default;

		size_t getVertexSize() const;

		size_t positionOffset() const;
		size_t normalOffset() const;
		size_t tangentOffset() const;
		size_t texcoordOffset(size_t i) const;
		size_t colorOffset(size_t i) const;


	public:
		Format positionFormat, normalFormat, tangentFormat;
		std::vector<Format> texcoordFormats, colorFormats;

	};

}
