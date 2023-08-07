#include "ComplexGeometry.h"

namespace mathem
{
	ComplexGeometry::ComplexGeometry(GeometryType type)
	{
		setType(type);
	}

	void ComplexGeometry::setType(GeometryType type)
	{
		this->type = type;
		this->volume = 0.0f;
		this->primitives.clear();
	}

	GeometryType ComplexGeometry::getType()
	{
		return type;
	}

	float ComplexGeometry::getVolume()
	{
		return volume;
	}

	size_t ComplexGeometry::getPrimitivesCount()
	{
		return primitives.getElementsCount();
	}

	GeometryPrimitiveBase* ComplexGeometry::getPrimitive(size_t index)
	{
		return primitives.getElement(index);
	}

	void ComplexGeometry::reservePrimitives(size_t count)
	{
		primitives.reserve(count);
	}

	void ComplexGeometry::appendPrimitive(GeometryPrimitiveBase* geometryPrimitive)
	{
		// TODO: add volume
		primitives.appendElement(geometryPrimitive);
	}
}