#include "DXUtils.h"

using namespace DXUtils;

const AggregateType AggregateType::SCALAR(1);
const AggregateType AggregateType::VEC2(2);
const AggregateType AggregateType::VEC3(3);
const AggregateType AggregateType::VEC4(4);
const AggregateType AggregateType::MAT2(4);
const AggregateType AggregateType::MAT3(9);
const AggregateType AggregateType::MAT4(16);

DXUtils::AggregateType::AggregateType() : count(0) {}

DXUtils::AggregateType::AggregateType(size_t count) : count(count) {}

size_t DXUtils::AggregateType::getCount() const {
	return count;
}

const ComponentType ComponentType::BYTE(1);
const ComponentType ComponentType::UNSIGNED_BYTE(1);
const ComponentType ComponentType::SHORT(2);
const ComponentType ComponentType::UNSIGNED_SHORT(2);
const ComponentType ComponentType::UNSIGNED_INT(4);
const ComponentType ComponentType::FLOAT(4);

DXUtils::ComponentType::ComponentType() : bytes(0) {}

DXUtils::ComponentType::ComponentType(size_t bytes) : bytes(bytes) {}

size_t DXUtils::ComponentType::getBytes() const {
	return bytes;
}

DXUtils::Format::Format(AggregateType aggregateType, ComponentType componentType) :
	aggregateType(aggregateType),
	componentType(componentType) {}

size_t DXUtils::Format::getSize() const {
	return aggregateType.getCount() * componentType.getBytes();
}
