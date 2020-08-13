#include "SceneLoading.h"

#include <unordered_map>

using namespace GLTF;

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
