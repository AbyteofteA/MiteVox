#include "computeContactPoints.h"

#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/Math/src/almostEqual.h"

namespace mathem
{
	bool isBeforePlane(Vector3D vertex, Vector3D planeNormal, float planeDistance);
	Vector3D projectOntoPlaneIfBehind(Vector3D vertex, Vector3D planeNormal, float planeDistance);
	size_t getFurthestVertexInTheDirection(GeometryPrimitiveBase* meshGeometry, GeometryTransform* meshGeometryTransform, Vector3D direction);

	void computeMeshContactPoints(
		GeometryPrimitiveBase* meshGeometry1,
		GeometryTransform* meshGeometryTransform1,
		GeometryPrimitiveBase* meshGeometry2,
		GeometryTransform* meshGeometryTransform2,
		CollisionProperties* collisionProperties,
		float equalityTolerance,
		bool isFirstPass = true)
	{
		Vector3D normal = collisionProperties->normal;
		Vector3D faceVerteces[4];
		size_t faceVertecesCount = 0;

		GeometryPrimitiveBase* referenceMeshGeometry;
		GeometryTransform* referenceMeshGeometryTransform;
		GeometryPrimitiveBase* incidentMeshGeometry;
		GeometryTransform* incidentMeshGeometryTransform;

		if (collisionProperties->normalBelongsToTheFirst)
		{
			if (collisionProperties->penetrationDepth > 0.0f)
			{
				normal = -normal;
			}

			referenceMeshGeometry = meshGeometry1;
			referenceMeshGeometryTransform = meshGeometryTransform1;
			incidentMeshGeometry = meshGeometry2;
			incidentMeshGeometryTransform = meshGeometryTransform2;
		}
		else
		{
			referenceMeshGeometry = meshGeometry2;
			referenceMeshGeometryTransform = meshGeometryTransform2;
			incidentMeshGeometry = meshGeometry1;
			incidentMeshGeometryTransform = meshGeometryTransform1;
		}

		// Plane calculation
		size_t furthestVertexIndex = getFurthestVertexInTheDirection(referenceMeshGeometry, referenceMeshGeometryTransform, normal);
		Vector3D planeVertex = referenceMeshGeometry->getVertexPosition(furthestVertexIndex);
		referenceMeshGeometryTransform->applyTo(planeVertex);
		float planeDistance = planeVertex * normal;

		// Find contact face
		size_t closestVertexIndex = getFurthestVertexInTheDirection(incidentMeshGeometry, incidentMeshGeometryTransform, -normal);
		faceVertecesCount = incidentMeshGeometry->getFaceVerteces(
			closestVertexIndex, -normal, faceVerteces, incidentMeshGeometryTransform, equalityTolerance);

		float distance = 0.01;
		for (size_t i = 0; i < faceVertecesCount; i++)
		{
			Vector3D faceVertex = faceVerteces[i];
			
			// Project onto reference geometry
			size_t referenceTrianglesCount = referenceMeshGeometry->getTrianglesCount();
			for (size_t i = 0; i < referenceTrianglesCount; ++i)
			{
				TriangleGeometry3D resultTriangle = referenceMeshGeometry->getTrianglePositions(i);
				referenceMeshGeometryTransform->applyTo(resultTriangle);
				Vector3D triangleNormal = resultTriangle.computeNormal();
				if (almostEqual(triangleNormal, normal, equalityTolerance))
				{
					continue;
				}

				triangleNormal = -triangleNormal;
				float tmpPlaneDistance = resultTriangle.point1 * triangleNormal;
				faceVertex = projectOntoPlaneIfBehind(faceVertex, triangleNormal, tmpPlaneDistance);
			}

			// Project onto incident geometry
			size_t incidentTrianglesCount = incidentMeshGeometry->getTrianglesCount();
			for (size_t i = 0; i < incidentTrianglesCount; ++i)
			{
				TriangleGeometry3D resultTriangle = incidentMeshGeometry->getTrianglePositions(i);
				incidentMeshGeometryTransform->applyTo(resultTriangle);
				Vector3D triangleNormal = -resultTriangle.computeNormal();
				float tmpPlaneDistance = resultTriangle.point1 * triangleNormal;
				faceVertex = projectOntoPlaneIfBehind(faceVertex, triangleNormal, tmpPlaneDistance);
			}

			// Discard points above reference surfase
			if (isBeforePlane(faceVertex, normal, planeDistance))
			{
				continue;
			}

			collisionProperties->tryAddNewContactPoint(faceVertex, distance, equalityTolerance);
			distance *= 0.01;
		}
	}

	// TODO: checkCollision BOX vs AXISALIGNEDBOX
	// TODO: checkCollision BOX vs SPHERE
	// TODO: checkCollision BOX vs CAPSULE
	// TODO: checkCollision BOX vs TRUNCATEDPYRAMID
	// TODO: checkCollision BOX vs MESH
	// TODO: checkCollision BOX vs RAY

	void computeContactPoints(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties,
		float equalityTolerance)
	{
		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
		case GeometryPrimitiveType::AXIS_ALIGNED_BOX:
		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
		case GeometryPrimitiveType::MESH:
			computeMeshContactPoints(
				(GeometryPrimitiveBase*)box, 
				boxTransform, 
				(GeometryPrimitiveBase*)otherGeometry, 
				otherGeometryTransform, 
				collisionProperties, 
				equalityTolerance);
			break;

		case GeometryPrimitiveType::SPHERE:
			// TODO: computeContactPoints(box, boxTransform, (SphereGeometry*)otherGeometry, otherGeometryTransform, collisionProperties, equalityTolerance);
			break;
		case GeometryPrimitiveType::CAPSULE:
			// TODO: computeContactPoints(box, boxTransform, (CapsuleGeometry*)otherGeometry, otherGeometryTransform, collisionProperties, equalityTolerance);
			break;
		default:
			break;
		}
	}

	bool isBeforePlane(Vector3D vertex, Vector3D planeNormal, float planeDistance)
	{
		float vertexProjection = vertex * planeNormal;
		if (vertexProjection <= planeDistance)
		{
			return false;
		}
		return true;
	}

	Vector3D projectOntoPlaneIfBehind(Vector3D vertex, Vector3D planeNormal, float planeDistance)
	{
		float vertexProjection = vertex * planeNormal;
		if (vertexProjection >= planeDistance)
		{
			return vertex;
		}
		Vector3D difference = planeNormal * (planeDistance - vertexProjection);
		return vertex + difference;
	}

	size_t getFurthestVertexInTheDirection(GeometryPrimitiveBase* meshGeometry, GeometryTransform* meshGeometryTransform, Vector3D direction)
	{
		size_t furthestVertexIndex = 0;
		float maxDistance = -std::numeric_limits<float>::max();
		size_t meshGeometryVertecesCount = meshGeometry->getVertecesCount();
		for (size_t i = 0; i < meshGeometryVertecesCount; i++)
		{
			Vector3D vertex = meshGeometry->getVertexPosition(i);
			meshGeometryTransform->applyTo(vertex);
			float vertexDistance = vertex * direction;
			if (maxDistance < vertexDistance)
			{
				maxDistance = vertexDistance;
				furthestVertexIndex = i;
			}
		}
		return furthestVertexIndex;
	}
}