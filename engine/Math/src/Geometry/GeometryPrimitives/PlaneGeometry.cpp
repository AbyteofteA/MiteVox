#include "PlaneGeometry.h"

#include "engine/Math/src/NumericalAnalysis/Intertolation.h"

namespace mathem
{
	float PlaneGeometry::getSignedDistance(Vector3D vertex)
	{
		float vertexProjection = vertex * normal;
		return vertexProjection - position;
	}

	bool PlaneGeometry::isBeforePlane(Vector3D vertex)
	{
		if (getSignedDistance(vertex) > 0.0f)
		{
			return true;
		}
		return false;
	}

	Vector3D PlaneGeometry::projectOntoPlaneIfBehind(Vector3D vertex)
	{
		float vertexProjection = vertex * normal;
		if (vertexProjection >= position)
		{
			return vertex;
		}
		Vector3D difference = normal * (position - vertexProjection);
		return vertex + difference;
	}
}