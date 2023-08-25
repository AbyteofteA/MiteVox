#ifndef COMPLEXGEOMETRY_H
#define COMPLEXGEOMETRY_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"

namespace mathem
{
	enum struct GeometryType : uint8_t
	{
		NONE = 0,			/// 
		POINT,				/// 
		PRIMITIVES,			///
		MESH,				/// 
		SKELETON			/// 
	};

	class ComplexGeometry
	{
	public:

		ComplexGeometry(GeometryType type);
		/// <summary>
		/// Sets geometry type and clears all previous data
		/// </summary>
		void setType(GeometryType type);

		GeometryType getType();

		float getVolume();
		size_t getPrimitivesCount();
		GeometryPrimitiveBase* getPrimitive(size_t index);
		void reservePrimitives(size_t count);
		void appendPrimitive(GeometryPrimitiveBase geometryPrimitive);
		//void tryMake

	private:

		GeometryType type = GeometryType::NONE;
		safety::SafeArray<GeometryPrimitiveBase> primitives;
		float volume = 0.0f;
	};
}

#endif