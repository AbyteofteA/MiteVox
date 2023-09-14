#include "computeProjection.h"

#include <utility>

namespace mathem
{
	void computeProjectionForMesh(
		mitevox::Mesh* mesh,
		Vector3D* vector,
		float* min,
		float* max)
	{
		Vector3D point = mesh->getVertexPosition(0);
		*max = point * (*vector); // Compute projection of the first point
		*min = *max;
		size_t vertecesCount = mesh->getVertecesCount();
		for (size_t i = 1; i < vertecesCount; ++i)
		{
			point = mesh->getVertexPosition(i);
			float pointProjection = point * (*vector);

			if (pointProjection < *min)
			{
				*min = pointProjection;
			}
			if (pointProjection > *max)
			{
				*max = pointProjection;
			}
		}
	}

	void computeProjectionForMesh(
		mitevox::MeshPrimitive* meshPrimitive,
		Vector3D* vector,
		float* min,
		float* max)
	{
		Vector3D point = meshPrimitive->getVertexPosition(0);
		*max = point * (*vector); // Compute projection of the first point
		*min = *max;
		size_t vertecesCount = meshPrimitive->getVertecesCount();
		for (size_t i = 1; i < vertecesCount; ++i)
		{
			point = meshPrimitive->getVertexPosition(i);
			float pointProjection = point * (*vector);

			if (pointProjection < *min)
			{
				*min = pointProjection;
			}
			if (pointProjection > *max)
			{
				*max = pointProjection;
			}
		}
	}

	void computeProjection(
		GeometryPrimitiveBase* triangularMesh,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max)
	{
		Vector3D point = triangularMesh->getVertexPosition(0);
		parentTransform->applyTo(point);
		*max = point * (*vector); // Compute projection of the first point
		*min = *max;
		size_t vertecesCount = triangularMesh->getVertecesCount();
		for (size_t i = 1; i < vertecesCount; ++i)
		{
			point = triangularMesh->getVertexPosition(i);
			parentTransform->applyTo(point);
			float pointProjection = point * (*vector);

			if (pointProjection < *min)
			{
				*min = pointProjection;
			}
			if (pointProjection > *max)
			{
				*max = pointProjection;
			}
		}
	}

	void computeProjection(
		SphereGeometry* sphere, 
		GeometryTransform* parentTransform,
		Vector3D* vector, 
		float* min, 
		float* max)
	{
		Vector3D spherePosition = sphere->transform.translation;
		parentTransform->applyTo(spherePosition);
		float spherePositionProjection = spherePosition * (*vector);
		float actualSphereRadius = sphere->radius * sphere->transform.scale.x() * parentTransform->scale.x();
		*min = spherePositionProjection - actualSphereRadius;
		*max = spherePositionProjection + actualSphereRadius;
	}

	void computeProjection(
		CapsuleGeometry* capsule,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max)
	{
		float actualCapsuleHalfHeight = capsule->halfHeight * capsule->transform.scale.x();
		Vector3D capsulePosition1 = capsule->transform.translation;
		Vector3D capsulePosition2 = capsule->transform.translation;
		capsulePosition1.y() += actualCapsuleHalfHeight;
		capsulePosition2.y() -= actualCapsuleHalfHeight;
		parentTransform->applyTo(capsulePosition1);
		parentTransform->applyTo(capsulePosition2);

		float spherePositionProjectionMin = capsulePosition1 * (*vector);
		float spherePositionProjectionMax = capsulePosition2 * (*vector);
		if (spherePositionProjectionMax < spherePositionProjectionMin)
		{
			std::swap(spherePositionProjectionMin, spherePositionProjectionMax);
		}
		float actualCapsuleRadius = capsule->radius * capsule->transform.scale.x() * parentTransform->scale.x();
		*min = spherePositionProjectionMin - actualCapsuleRadius;
		*max = spherePositionProjectionMax + actualCapsuleRadius;
	}

	void computeProjection(
		TriangleGeometry3D* triangle,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max)
	{
		Vector3D trianglePoint1 = triangle->getPoint1();
		Vector3D trianglePoint2 = triangle->getPoint2();
		Vector3D trianglePoint3 = triangle->getPoint3();
		parentTransform->applyTo(trianglePoint1);
		parentTransform->applyTo(trianglePoint2);
		parentTransform->applyTo(trianglePoint3);

		float triangleProjection1 = trianglePoint1 * (*vector);
		float triangleProjection2 = trianglePoint2 * (*vector);
		float triangleProjection3 = trianglePoint3 * (*vector);

		if (triangleProjection1 > triangleProjection2)
		{
			std::swap(triangleProjection1, triangleProjection2);
		}

		if (triangleProjection2 > triangleProjection3)
		{
			std::swap(triangleProjection2, triangleProjection3);
		}

		if (triangleProjection1 > triangleProjection2)
		{
			std::swap(triangleProjection1, triangleProjection2);
		}

		*min = triangleProjection1;
		*max = triangleProjection3;
	}
}