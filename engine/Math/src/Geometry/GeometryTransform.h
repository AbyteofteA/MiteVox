
#ifndef GEOMETRYTRANSFORM_H
#define GEOMETRYTRANSFORM_H

#include "engine/Math/src/Vector.h"

namespace mathem
{
	class GeometryTransform
	{
	public:

		Vector3D position;
		Vector4D orientation; /// TODO: move to quaternions
		Vector3D scale = { 1.0f, 1.0f, 1.0f };
	};
}

#endif
