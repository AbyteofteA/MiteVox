#include "GeometryPrimitiveBase.h"

#include "engine/Math/src/almostEqual.h"
#include "engine/Math/src/MinAndMax.h"

namespace mathem
{
	GeometryPrimitiveType GeometryPrimitiveBase::getType()
	{
		return type;
	}

	bool containsVertex(Vector3D vertex, Vector3D* faceVerteces, size_t faceVertecesCount)
	{
		for (size_t i = 0; i < faceVertecesCount; ++i)
		{
			if (faceVerteces[i] == vertex)
			{
				return true;
			}
		}
		return false;
	}

	void tryAppendVertex(Vector3D vertex, Vector3D* faceVerteces, size_t* faceVertecesCount, float equalityTolerance)
	{
		// Check if contains the vertex
		for (size_t i = 0; i < *faceVertecesCount; ++i)
		{
			if (almostEqual(faceVerteces[i], vertex, equalityTolerance))
			{
				return;
			}
		}
		// Check if the array is full
		if (*faceVertecesCount >= 4)
		{
			return;
		}
		faceVerteces[*faceVertecesCount] = vertex;
		*faceVertecesCount += 1;
	}

	size_t GeometryPrimitiveBase::getFaceVerteces(size_t vertexIndex, Vector3D normal, Vector3D* faceVerteces, GeometryTransform* geometryPrimitiveTransform, float equalityTolerance)
	{
		Vector3D targetNormal;
		Vector3D targetVertex = getVertexPosition(vertexIndex);
		float normalAngle = -mathem::max<float>();
		size_t trianglesCount = getTrianglesCount();
		for (size_t i = 0; i < trianglesCount; ++i)
		{
			TriangleGeometry3D resultTriangle = getTrianglePositions(i);
			if (resultTriangle.point1 != targetVertex &&
				resultTriangle.point2 != targetVertex &&
				resultTriangle.point3 != targetVertex)
			{
				continue;
			}
			geometryPrimitiveTransform->applyTo(resultTriangle);
			Vector3D triangleNormal = resultTriangle.computeNormal();
			float newNormalAngle = (triangleNormal * normal);
			if (newNormalAngle > normalAngle)
			{
				normalAngle = newNormalAngle;
				targetNormal = triangleNormal;
				continue;
			}
		}

		size_t faceVertecesCount = 0;

		for (size_t i = 0; i < trianglesCount; ++i)
		{
			TriangleGeometry3D resultTriangle = getTrianglePositions(i);
			geometryPrimitiveTransform->applyTo(resultTriangle);
			Vector3D triangleNormal = resultTriangle.computeNormal();
			if (almostEqual(targetNormal, triangleNormal, equalityTolerance) == false)
			{
				continue;
			}

			tryAppendVertex(resultTriangle.point1, faceVerteces, &faceVertecesCount, equalityTolerance);
			tryAppendVertex(resultTriangle.point2, faceVerteces, &faceVertecesCount, equalityTolerance);
			tryAppendVertex(resultTriangle.point3, faceVerteces, &faceVertecesCount, equalityTolerance);
		}
		return faceVertecesCount;
	}
}