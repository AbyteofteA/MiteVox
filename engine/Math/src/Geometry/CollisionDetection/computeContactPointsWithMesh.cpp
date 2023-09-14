#include "computeContactPoints.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/PlaneGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Utilities/almostEqual.h"
#include "engine/Math/src/Utilities/MinAndMax.h"

namespace mathem
{
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
		PlaneGeometry referencePlane;
		referencePlane.normal = collisionProperties->normal;
		size_t furthestVertexIndex = getFurthestVertexInTheDirection(referenceMeshGeometry, referenceMeshGeometryTransform, referencePlane.normal);
		Vector3D planeVertex = referenceMeshGeometry->getVertexPosition(furthestVertexIndex);
		referenceMeshGeometryTransform->applyTo(planeVertex);
		referencePlane.position = planeVertex * referencePlane.normal;

		// Find contact face
		size_t closestVertexIndex = getFurthestVertexInTheDirection(incidentMeshGeometry, incidentMeshGeometryTransform, -referencePlane.normal);
		Vector3D faceVerteces[4];
		size_t faceVertecesCount = 0;
		faceVertecesCount = incidentMeshGeometry->getFaceVerteces(
			closestVertexIndex, -referencePlane.normal, faceVerteces, incidentMeshGeometryTransform, equalityTolerance);

		float distance = 0.01f;
		for (size_t i = 0; i < faceVertecesCount; i++)
		{
			Vector3D faceVertex = faceVerteces[i];
			
			// Discard points above reference surfase
			if (referencePlane.isBeforePlane(faceVertex))
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
				if (almostEqual(triangleNormal, referencePlane.normal, equalityTolerance))
				{
					continue;
				}

				PlaneGeometry tmpPlane;
				tmpPlane.normal = -triangleNormal;
				tmpPlane.position = resultTriangle.getPoint1() * tmpPlane.normal;
				faceVertex = tmpPlane.projectOntoPlaneIfBehind(faceVertex);
			}

			Vector3D referenceContactPoint = referencePlane.projectOntoPlaneIfBehind(faceVertex);
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

	void computeContactPointsWithMesh(
		GeometryPrimitiveBase* mesh,
		GeometryTransform* meshTransform,
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
		case GeometryPrimitiveType::CONVEX_HULL:
			computeContactPointsMeshVsMesh(
				mesh,
				meshTransform,
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