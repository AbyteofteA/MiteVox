
#ifndef GEOMETRYTRANSFORM_H
#define GEOMETRYTRANSFORM_H

#include "engine/Math/src/LinearAlgebra/Vector3D.h"

namespace mathem
{
	class GeometryTransform
	{
	public:

		Vector3D position = { 0.f };
		Vector3D orientation = { 0.f }; /// TODO: move to quaternions
		Vector3D scale = { 1.f };
	};
}

#endif
