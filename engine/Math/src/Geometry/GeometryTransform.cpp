#include "GeometryTransform.h"

namespace mathem
{
	GeometryTransform::GeometryTransform()
	{

	}

	GeometryTransform::GeometryTransform(safety::SafeFloatArray* columnMajorArray4x4)
	{
		// TODO:
	}

	void GeometryTransform::reset()
	{
		translation = { 0.0f, 0.0f, 0.0f };
		rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		scale = { 1.0f, 1.0f, 1.0f };
	}

	void GeometryTransform::fromArray4x4(safety::SafeFloatArray* columnMajorMatrix4x4)
	{
		// TODO:
	}

	void GeometryTransform::fromScaleArray(safety::SafeFloatArray* scaleArray)
	{
		scale.x() = scaleArray->getElement(0);
		scale.y() = scaleArray->getElement(1);
		scale.z() = scaleArray->getElement(2);
	}

	void GeometryTransform::fromRotationArray(safety::SafeFloatArray* rotationArray)
	{
		rotation.x() = rotationArray->getElement(0);
		rotation.y() = rotationArray->getElement(1);
		rotation.z() = rotationArray->getElement(2);
		rotation.s() = rotationArray->getElement(3);
	}

	void GeometryTransform::fromTranslationArray(safety::SafeFloatArray* translationArray)
	{
		translation.x() = translationArray->getElement(0);
		translation.y() = translationArray->getElement(1);
		translation.z() = translationArray->getElement(2);
	}
}