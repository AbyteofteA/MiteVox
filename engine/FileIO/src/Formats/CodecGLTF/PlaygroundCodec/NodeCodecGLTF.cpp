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

	void NodeCodec::toGLTF(
		JSON* nodeJSONResult,
		mitevox::Node* node,
		safety::SafeArray<render::Camera*>* cameras,
		safety::SafeArray<mitevox::Mesh*>* meshes,
		safety::SafeArray<mitevox::Node*>* nodes,
		safety::SafeArray<mitevox::SkeletonBase*>* skeletons)
	{
		nodeJSONResult->setField("name", node->name);

		if (node->camera)
		{
			size_t camerasCount = cameras->getElementsCount();
			for (size_t i = 0; i < camerasCount; ++i)
			{
				if (node->camera == cameras->getElement(i))
				{
					nodeJSONResult->setField("camera", (double)i);
					break;
				}
			}
		}
		if (node->skeleton)
		{
			size_t skeletonsCount = skeletons->getElementsCount();
			for (size_t i = 0; i < skeletonsCount; ++i)
			{
				if (node->skeleton == skeletons->getElement(i))
				{
					nodeJSONResult->setField("skin", (double)i);
					break;
				}
			}
		}
		if (node->mesh)
		{
			size_t meshesCount = meshes->getElementsCount();
			for (size_t i = 0; i < meshesCount; ++i)
			{
				if (node->mesh == meshes->getElement(i))
				{
					nodeJSONResult->setField("mesh", (double)i);
					break;
				}
			}

			if (node->mesh->isMorphable())
			{
				JSON* weightsArrayJSON = nodeJSONResult->setFieldType("weights", JSONtype::ARRAY);
				weightsArrayJSON->fromNumberArray<float>(&node->weights);
			}
		}

		mathem::Vector3D identity = { 1.0f, 1.0f, 1.0f };
		mathem::Vector3D zero = { 0.0f, 0.0f, 0.0f };

		if (node->transform.scale != identity)
		{
			JSON* scaleArrayJSON = nodeJSONResult->setFieldType("scale", JSONtype::ARRAY);
			scaleArrayJSON->addArrayItem((double)node->transform.scale.x());
			scaleArrayJSON->addArrayItem((double)node->transform.scale.y());
			scaleArrayJSON->addArrayItem((double)node->transform.scale.z());
		}
		
		if (node->transform.rotation.isIdentity() == false)
		{
			JSON* rotationArrayJSON = nodeJSONResult->setFieldType("rotation", JSONtype::ARRAY);
			rotationArrayJSON->addArrayItem((double)node->transform.rotation.components.x());
			rotationArrayJSON->addArrayItem((double)node->transform.rotation.components.y());
			rotationArrayJSON->addArrayItem((double)node->transform.rotation.components.z());
			rotationArrayJSON->addArrayItem((double)node->transform.rotation.components.s());
		}

		if (node->transform.translation != zero)
		{
			JSON* translationArrayJSON = nodeJSONResult->setFieldType("translation", JSONtype::ARRAY);
			translationArrayJSON->addArrayItem((double)node->transform.translation.x());
			translationArrayJSON->addArrayItem((double)node->transform.translation.y());
			translationArrayJSON->addArrayItem((double)node->transform.translation.z());
		}

		size_t nodesCount = nodes->getElementsCount();
		size_t childrenCount = node->children.getElementsCount();
		if (childrenCount > 0)
		{
			JSON* childrenArrayJSON = nodeJSONResult->setFieldType("children", JSONtype::ARRAY);

			for (size_t childNodeIndex = 0; childNodeIndex < childrenCount; ++childNodeIndex)
			{
				for (size_t nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex)
				{
					if (node->children.getElement(childNodeIndex) == nodes->getElement(nodeIndex))
					{
						childrenArrayJSON->addArrayItem((double)nodeIndex);
						break;
					}
				}
			}
		}
	}
}