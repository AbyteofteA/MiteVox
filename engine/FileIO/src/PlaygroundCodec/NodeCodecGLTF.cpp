#include "NodeCodec.h"

#include "engine/Math/src/Convertations.h"

namespace fileio
{
	void NodeCodec::fromGLTF(
		mitevox::Node* nodeResult,
		JSON* nodeJSON,
		safety::SafeArray<render::Camera*>* cameras,
		safety::SafeArray<mitevox::Mesh*>* meshes,
		safety::SafeArray<mitevox::Node*>* nodes)
	{
		nodeResult->name = nodeJSON->getFieldString("name");

		JSON* numberJSON = nodeJSON->getField("camera");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t cameraIndex = (int32_t)numberJSON->getNumber();
				nodeResult->camera = cameras->getElement(cameraIndex);
			}
		}
		numberJSON = nodeJSON->getField("skin");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				size_t skeletonIndex = (size_t)numberJSON->getNumber() + 1;
				nodeResult->skeleton = (mitevox::SkeletonBase*)skeletonIndex;
			}
		}
		numberJSON = nodeJSON->getField("mesh");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t meshIndex = (int32_t)numberJSON->getNumber();
				nodeResult->mesh = meshes->getElement(meshIndex);
			}
		}
		JSON* weightsArrayJSON = nodeJSON->getFieldArray("weights");
		if (weightsArrayJSON != nullptr)
		{
			weightsArrayJSON->toNumberArray<float>(&nodeResult->weights);
		}

		nodeResult->transform.reset();

		JSON* scaleArrayJSON = nodeJSON->getFieldArray("scale");
		if (scaleArrayJSON != nullptr)
		{
			safety::SafeFloatArray scaleArray;
			scaleArrayJSON->toNumberArray<float>(&scaleArray);
			nodeResult->transform.scale = mathem::Vector3D(&scaleArray);
		}
		JSON* rotationArrayJSON = nodeJSON->getFieldArray("rotation");
		if (rotationArrayJSON != nullptr)
		{
			safety::SafeFloatArray rotationArray;
			rotationArrayJSON->toNumberArray<float>(&rotationArray);
			nodeResult->transform.rotation = mathem::Quaternion(&rotationArray);
		}
		JSON* translationArrayJSON = nodeJSON->getFieldArray("translation");
		if (translationArrayJSON != nullptr)
		{
			safety::SafeFloatArray translationArray;
			translationArrayJSON->toNumberArray<float>(&translationArray);
			nodeResult->transform.translation = mathem::Vector3D(&translationArray);
		}
		JSON* matrixArrayJSON = nodeJSON->getFieldArray("matrix");
		if (matrixArrayJSON != nullptr)
		{
			safety::SafeFloatArray transformationMatrixArray;
			matrixArrayJSON->toNumberArray<float>(&transformationMatrixArray);
			nodeResult->transform = mathem::matrixToTransform(mathem::Matrix4x4(&transformationMatrixArray));
		}

		JSON* childrenArrayJSON = nodeJSON->getFieldArray("children");
		if (childrenArrayJSON != nullptr)
		{
			size_t childrenCount = childrenArrayJSON->getArraySize();
			nodeResult->children.resize(childrenCount);
			nodeResult->children.fillWithZeros();

			for (size_t i = 0; i < childrenCount; ++i)
			{
				int32_t childIndex = (int32_t)childrenArrayJSON->getArrayItemNumber(i);
				nodeResult->children.setElement(i, nodes->getElement(childIndex));
			}
		}
	}
}