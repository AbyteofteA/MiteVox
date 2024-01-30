#include "Transform.h"

#ifndef TRANSFORM_REGION_HALF_SIZE
#define TRANSFORM_REGION_HALF_SIZE (500.0f)
#endif

namespace mitevox
{
	const float Transform::REGION_SIZE = TRANSFORM_REGION_HALF_SIZE * 2.0f;
	const float Transform::RECIPROCAL_REGION_SIZE = 1.0f / Transform::REGION_SIZE;
	const float Transform::REGION_HALF_SIZE = TRANSFORM_REGION_HALF_SIZE;
	const float Transform::RECIPROCAL_REGION_HALF_SIZE = 1.0f / Transform::REGION_HALF_SIZE;

	void Transform::init()
	{
		regionCoord = { 0, 0, 0 };
		updateCoordinatesHelper(regionCoord, localTransform.translation);
	}

	void Transform::reset()
	{
		regionCoord = { 0, 0, 0 };
		localTransform.reset();
	}

	void Transform::updateCoordinates()
	{
		updateCoordinatesHelper(regionCoord, localTransform.translation);
	}

	/// <summary>
	/// Computes relative GeometryTransform considering only translation, orientation remains the same.
	/// It is used for computing transforms relative to a camera.
	/// </summary>
	/// <param name="originTransform"></param>
	/// <returns></returns>
	mathem::GeometryTransform* Transform::computeRelativeTransform(Transform* originTransform)
	{
		RegionCoord relativeRegionCoord = regionCoord - originTransform->regionCoord;

		relativeTransform = localTransform;
		relativeTransform.translation = this->localTransform.translation - originTransform->localTransform.translation;

		relativeTransform.translation.x() += (float)relativeRegionCoord.x() * REGION_SIZE;
		relativeTransform.translation.y() += (float)relativeRegionCoord.y() * REGION_SIZE;
		relativeTransform.translation.z() += (float)relativeRegionCoord.z() * REGION_SIZE;

		return &relativeTransform;
	}

	void Transform::applyTo(Transform* otherTransform)
	{
		mathem::Vector3D regionCoordRotated;
		regionCoordRotated.x() = (float)otherTransform->regionCoord.x();
		regionCoordRotated.y() = (float)otherTransform->regionCoord.y();
		regionCoordRotated.z() = (float)otherTransform->regionCoord.z();
		regionCoordRotated += otherTransform->localTransform.translation;
		regionCoordRotated = this->localTransform.rotation.rotate(regionCoordRotated);
		mathem::Vector3D regionCoordTrunc;
		regionCoordTrunc.x() = std::trunc(regionCoordRotated.x());
		regionCoordTrunc.y() = std::trunc(regionCoordRotated.y());
		regionCoordTrunc.z() = std::trunc(regionCoordRotated.z());
		otherTransform->regionCoord.x() = (RegionCoordType)regionCoordTrunc.x();
		otherTransform->regionCoord.y() = (RegionCoordType)regionCoordTrunc.y();
		otherTransform->regionCoord.z() = (RegionCoordType)regionCoordTrunc.z();
		otherTransform->localTransform.translation = this->localTransform.rotation.rotate(otherTransform->localTransform.translation);
		otherTransform->localTransform.translation.x() += (regionCoordRotated.x() - regionCoordTrunc.x()) * REGION_SIZE;
		otherTransform->localTransform.translation.y() += (regionCoordRotated.y() - regionCoordTrunc.y()) * REGION_SIZE;
		otherTransform->localTransform.translation.z() += (regionCoordRotated.z() - regionCoordTrunc.z()) * REGION_SIZE;

		this->localTransform.rotation.rotate(otherTransform->localTransform.rotation);
	}

	mathem::GeometryTransform* Transform::getRelativeTransform()
	{
		return &relativeTransform;
	}

	mathem::Vector3D& Transform::getPosition()
	{
		return this->localTransform.translation;
	}

	mathem::Quaternion& Transform::getOrientation()
	{
		return this->localTransform.rotation;
	}

	RegionCoord& Transform::getRegionCoord()
	{
		return this->regionCoord;
	}

	void Transform::setPosition(mathem::Vector3D position)
	{
		this->regionCoord = { 0, 0, 0 };
		this->localTransform.translation = position;
	}

	void Transform::setPosition(RegionCoord regionCoord, mathem::Vector3D position)
	{
		this->regionCoord = regionCoord;
		this->localTransform.translation = position;
	}

	void Transform::addPosition(mathem::Vector3D position)
	{
		this->localTransform.translation += position;
	}

	void Transform::addPosition(RegionCoord regionCoord, mathem::Vector3D position)
	{
		this->regionCoord += regionCoord;
		this->localTransform.translation += position;
	}

	void Transform::setOrientation(mathem::Quaternion quaternion)
	{
		this->localTransform.rotation = quaternion;
	}

	void Transform::addOrientation(mathem::Quaternion quaternion)
	{
		this->localTransform.rotation.multiply(quaternion);
	}

	void Transform::addOrientation(mathem::Vector3D degrees)
	{
		this->localTransform.rotation.rotateByEulers(degrees.x(), degrees.y(), degrees.z());
	}

	void Transform::addOrientationRadians(mathem::Vector3D radians)
	{
		this->localTransform.rotation.rotateByEulersRadians(radians.x(), radians.y(), radians.z());
	}

	void Transform::updateCoordinatesHelper(RegionCoord& regionCoordinate, mathem::Vector3D& localTranslation)
	{
		float xTrunc = std::abs(localTranslation.x() * RECIPROCAL_REGION_HALF_SIZE);
		float yTrunc = std::abs(localTranslation.y() * RECIPROCAL_REGION_HALF_SIZE);
		float zTrunc = std::abs(localTranslation.z() * RECIPROCAL_REGION_HALF_SIZE);

		xTrunc = (xTrunc + 1.0f) * 0.5f;
		yTrunc = (yTrunc + 1.0f) * 0.5f;
		zTrunc = (zTrunc + 1.0f) * 0.5f;

		xTrunc = std::floor(xTrunc);
		yTrunc = std::floor(yTrunc);
		zTrunc = std::floor(zTrunc);

		xTrunc = std::copysign(xTrunc, localTranslation.x());
		yTrunc = std::copysign(yTrunc, localTranslation.y());
		zTrunc = std::copysign(zTrunc, localTranslation.z());

		regionCoordinate.x() += (RegionCoordType)xTrunc;
		regionCoordinate.y() += (RegionCoordType)yTrunc;
		regionCoordinate.z() += (RegionCoordType)zTrunc;

		localTranslation.x() -= xTrunc * REGION_SIZE;
		localTranslation.y() -= yTrunc * REGION_SIZE;
		localTranslation.z() -= zTrunc * REGION_SIZE;
	}
}