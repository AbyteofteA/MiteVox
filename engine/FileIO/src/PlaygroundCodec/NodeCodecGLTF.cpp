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
		nodeResult->name = nodeJSON->getFieldStringOrDefault("name", "Untitled");

		
		if (JSON* numberJSON = nodeJSON->getField("camera"))
		{
			int32_t cameraIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			nodeResult->camera = cameras->getElement(cameraIndex);
		}
		if (JSON* numberJSON = nodeJSON->getField("skin"))
		{
			int32_t skeletonIndex = (int32_t)numberJSON->getNumberOrDefault(-2.0) + 1;
			nodeResult->skeleton = (mitevox::SkeletonBase*)skeletonIndex;
		}
		if (JSON* numberJSON = nodeJSON->getField("mesh"))
		{
			int32_t meshIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			nodeResult->mesh = meshes->getElement(meshIndex);
		}
		if (JSON* weightsArrayJSON = nodeJSON->getFieldArray("weights"))
		{
			weightsArrayJSON->toNumberArray<float>(&nodeResult->weights);
		}

		nodeResult->transform.reset();

		if (JSON* scaleArrayJSON = nodeJSON->getFieldArray("scale"))
		{
			safety::SafeFloatArray scaleArray;
			scaleArrayJSON->toNumberArray<float>(&scaleArray);
			nodeResult->transform.scale = mathem::Vector3D(&scaleArray);
		}
		if (JSON* rotationArrayJSON = nodeJSON->getFieldArray("rotation"))
		{
			safety::SafeFloatArray rotationArray;
			rotationArrayJSON->toNumberArray<float>(&rotationArray);
			nodeResult->transform.rotation = mathem::Quaternion(&rotationArray);
		}
		if (JSON* translationArrayJSON = nodeJSON->getFieldArray("translation"))
		{
			safety::SafeFloatArray translationArray;
			translationArrayJSON->toNumberArray<float>(&translationArray);
			nodeResult->transform.translation = mathem::Vector3D(&translationArray);
		}
		if (JSON* matrixArrayJSON = nodeJSON->getFieldArray("matrix"))
		{
			safety::SafeFloatArray transformationMatrixArray;
			matrixArrayJSON->toNumberArray<float>(&transformationMatrixArray);
			mathem::Matrix4x4 nodeTransformMatrix = mathem::Matrix4x4(&transformationMatrixArray);
			nodeResult->transform = mathem::matrixToTransform(nodeTransformMatrix);
		}

		if (JSON* childrenArrayJSON = nodeJSON->getFieldArray("children"))
		{
			size_t childrenCount = childrenArrayJSON->getArraySize();
			nodeResult->children.resize(childrenCount);
			nodeResult->children.fillWithZeros();

			for (size_t i = 0; i < childrenCount; ++i)
			{
				int32_t childIndex = (int32_t)childrenArrayJSON->getArrayItemNumberOrDefault(i, -1.0);
				nodeResult->children.setElement(i, nodes->getElement(childIndex));
			}
		}
	}
}