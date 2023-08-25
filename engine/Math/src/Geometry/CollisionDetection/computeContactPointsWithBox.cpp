#include "computeContactPoints.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/almostEqual.h"
#include "engine/Math/src/MinAndMax.h"

namespace mathem
{
	bool isBeforePlane(Vector3D vertex, Vector3D planeNormal, float planeDistance);
	Vector3D projectOntoPlaneIfBehind(Vector3D vertex, Vector3D planeNormal, float planeDistance);
	size_t getFurthestVertexInTheDirection(GeometryPrimitiveBase* meshGeometry, GeometryTransform* meshGeometryTransform, Vector3D direction);

	void computeContactPointsMeshVsMesh(
		GeometryPrimitiveBase* meshGeometry1,
		GeometryTransform* meshGeometryTransform1,
		GeometryPrimitiveBase* meshGeometry2,
		GeometryTransform* meshGeometryTransform2,
		CollisionProperties* collisionProperties,
		float equalityTolerance,
		bool isFirstPass = true)
	{
		GeometryPrimitiveBase* referenceMeshGeometry = meshGeometry1;
		GeometryTransform* referenceMeshGeometryTransform = meshGeometryTransform1;
		GeometryPrimitiveBase* incidentMeshGeometry = meshGeometry2;
		GeometryTransform* incidentMeshGeometryTransform = meshGeometryTransform2;

		// Plane calculation
		Vector3D referencePlaneNormal = collisionProperties->normal;
		size_t furthestVertexIndex = getFurthestVertexInTheDirection(referenceMeshGeometry, referenceMeshGeometryTransform, referencePlaneNormal);
		Vector3D planeVertex = referenceMeshGeometry->getVertexPosition(furthestVertexIndex);
		referenceMeshGeometryTransform->applyTo(planeVertex);
		float referencePlaneDistance = planeVertex * referencePlaneNormal;

		// Find contact face
		size_t closestVertexIndex = getFurthestVertexInTheDirection(incidentMeshGeometry, incidentMeshGeometryTransform, -referencePlaneNormal);
		Vector3D faceVerteces[4];
		size_t faceVertecesCount = 0;
		faceVertecesCount = incidentMeshGeometry->getFaceVerteces(
			closestVertexIndex, -referencePlaneNormal, faceVerteces, incidentMeshGeometryTransform, equalityTolerance);

		float distance = 0.01f;
		for (size_t i = 0; i < faceVertecesCount; i++)
		{
			Vector3D faceVertex = faceVerteces[i];
			
			// Discard points above reference surfase
			if (isBeforePlane(faceVertex, referencePlaneNormal, referencePlaneDistance))
			{
				continue;
			}

			// Project onto reference geometry
			size_t referenceTrianglesCount = referenceMeshGeometry->getTrianglesCount();
			for (size_t i = 0; i < referenceTrianglesCount; ++i)
			{
				TriangleGeometry3D resultTriangle = referenceMeshGeometry->getTrianglePositions(i);
				referenceMeshGeometryTransform->applyTo(resultTriangle);
				Vector3D triangleNormal = resultTriangle.computeNormal();

				// Don't project onto reference plane
				if (almostEqual(triangleNormal, referencePlaneNormal, equalityTolerance))
				{
					continue;
				}

				triangleNormal = -triangleNormal;
				float tmpPlaneDistance = resultTriangle.point1 * triangleNormal;
				faceVertex = projectOntoPlaneIfBehind(faceVertex, triangleNormal, tmpPlaneDistance);
			}

			// Project onto incident geometry
			/*size_t incidentTrianglesCount = incidentMeshGeometry->getTrianglesCount();
			for (size_t i = 0; i < incidentTrianglesCount; ++i)
			{
				TriangleGeometry3D resultTriangle = incidentMeshGeometry->getTrianglePositions(i);
				incidentMeshGeometryTransform->applyTo(resultTriangle);
				Vector3D triangleNormal = -resultTriangle.computeNormal();
				float tmpPlaneDistance = resultTriangle.point1 * triangleNormal;
				faceVertex = projectOntoPlaneIfBehind(faceVertex, triangleNormal, tmpPlaneDistance);
			}*/

			Vector3D referenceContactPoint = projectOntoPlaneIfBehind(faceVertex, referencePlaneNormal, referencePlaneDistance);
			collisionProperties->tryAddNewContactPoint(
				referenceContactPoint, 
				faceVertex,
				distance, 
				equalityTolerance);
			distance *= 0.01f;
		}
	}

	// TODO: checkCollision BOX vs AXISALIGNEDBOX
	// TODO: checkCollision BOX vs SPHERE
	// TODO: checkCollision BOX vs CAPSULE
	// TODO: checkCollision BOX vs TRUNCATEDPYRAMID
	// TODO: checkCollision BOX vs MESH
	// TODO: checkCollision BOX vs RAY

	void computeContactPointsWithBox(
		GeometryPrimitiveBase* box,
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
			computeContactPointsMeshVsMesh(
				box, 
				boxTransform, 
				otherGeometry, 
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
		float maxDistance = -mathem::max<float>();
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