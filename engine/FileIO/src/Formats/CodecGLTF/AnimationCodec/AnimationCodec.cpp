#include "AnimationCodec.h"

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/MiteVox/src/Animation/Animation.h"
#include "engine/MiteVox/src/Animation/AnimationChannel.h"

namespace fileio
{
	void AnimationCodec::fromGLTF(
		mitevox::Animation* animationResult, 
		JSON* animationJSON,
		safety::SafeArray<mitevox::Node*>* nodes,
		safety::SafeArray<mitevox::BufferViewAccessor*>* accessors)
	{
		animationResult->_name = animationJSON->getFieldStringOrDefault("name", "Untitled");

		JSON* animationSamplersArrayJSON = animationJSON->getFieldArray("samplers");
		size_t animationSamplersCount = 0;
		if (animationSamplersArrayJSON != nullptr)
		{
			animationSamplersCount = animationSamplersArrayJSON->getArraySize();
		}

		if (JSON* animationChannelsArrayJSON = animationJSON->getFieldArray("channels"))
		{
			size_t animationChannelsCount = animationChannelsArrayJSON->getArraySize();
			// TODO: reserve animationChannels

			for (size_t i = 0; i < animationChannelsCount; ++i)
			{
				JSON* animationChannelJSON = animationChannelsArrayJSON->getArrayItem(i);

				int32_t samplerIndex = -1;
				if (JSON* numberJSON = animationChannelJSON->getField("sampler"))
				{
					samplerIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				}
				mitevox::BufferViewAccessor* timeAccessor = nullptr;
				mitevox::BufferViewAccessor* dataAccessor = nullptr;
				mitevox::InterpolationType interpolationType = mitevox::InterpolationType::LINEAR;
				size_t animationFramesCount = 0;
				if (JSON* animationSamplerJSON = animationSamplersArrayJSON->getArrayItem(samplerIndex))
				{
					if (JSON* numberJSON = animationSamplerJSON->getField("input"))
					{
						timeAccessor = accessors->getElement((size_t)numberJSON->getNumberOrDefault(-1.0));
						animationFramesCount = timeAccessor->count;
					}

					if (JSON* numberJSON = animationSamplerJSON->getField("output"))
					{
						dataAccessor = accessors->getElement((size_t)numberJSON->getNumberOrDefault(-1.0));
					}

					std::string interpolationTypeString = "LINEAR";
					if (JSON* interpolationTypeJSON = animationSamplerJSON->getField("interpolation"))
					{
						interpolationTypeString = interpolationTypeJSON->getStringOrDefault("LINEAR");
					}

					if (interpolationTypeString == "LINEAR")
					{
						interpolationType = mitevox::InterpolationType::LINEAR;
					}
					else if (interpolationTypeString == "STEP")
					{
						interpolationType = mitevox::InterpolationType::STEP;
					}
					else if (interpolationTypeString == "CUBICSPLINE")
					{
						interpolationType = mitevox::InterpolationType::CUBIC_SPLINE;
					}
					else
					{
						// TODO: error
					}
				}

				mitevox::AnimationChannelBase* animationChannelBase = nullptr;
				JSON* animationTargetJSON = animationChannelJSON->getField("target");
				std::string animationTargetPath = animationTargetJSON->getField("path")->getStringOrDefault("ERROR");

				if (JSON* animationTargetNodeJSON = animationTargetJSON->getField("node"))
				{
					int32_t animationTargetNodeIndex = (int32_t)animationTargetNodeJSON->getNumberOrDefault(-1.0);
					mitevox::Node* node = nodes->getElement(animationTargetNodeIndex);

					if (animationTargetPath == "translation")
					{
						auto animationChannel = new mitevox::AnimationChannel<mathem::Vector3D>(
							&node->transform.translation, interpolationType, animationFramesCount);

						for (size_t i = 0; i < animationFramesCount; ++i)
						{
							float time = timeAccessor->getElementsComponentAsFloat(i, 0);
							mathem::Vector3D translation;
							translation.x() = dataAccessor->getElementsComponentAsFloat(i, 0);
							translation.y() = dataAccessor->getElementsComponentAsFloat(i, 1);
							translation.z() = dataAccessor->getElementsComponentAsFloat(i, 2);
							animationChannel->setFrame(i, time, translation);
						}
						animationChannelBase = animationChannel;
					}
					else if (animationTargetPath == "rotation")
					{
						auto animationChannel = new mitevox::AnimationChannel<mathem::Quaternion>(
							&node->transform.rotation, interpolationType, animationFramesCount);
						for (size_t i = 0; i < animationFramesCount; ++i)
						{
							float time = timeAccessor->getElementsComponentAsFloat(i, 0);
							mathem::Quaternion rotation;
							rotation.binary.scalar = dataAccessor->getElementsComponentAsFloat(i, 3);
							rotation.binary.vector.x() = dataAccessor->getElementsComponentAsFloat(i, 0);
							rotation.binary.vector.y() = dataAccessor->getElementsComponentAsFloat(i, 1);
							rotation.binary.vector.z() = dataAccessor->getElementsComponentAsFloat(i, 2);
							animationChannel->setFrame(i, time, rotation);
						}
						animationChannelBase = animationChannel;
					}
					else if (animationTargetPath == "scale")
					{
						auto animationChannel = new mitevox::AnimationChannel<mathem::Vector3D>(
							&node->transform.scale, interpolationType, animationFramesCount);
						for (size_t i = 0; i < animationFramesCount; ++i)
						{
							float time = timeAccessor->getElementsComponentAsFloat(i, 0);
							mathem::Vector3D scale;
							scale.x() = dataAccessor->getElementsComponentAsFloat(i, 0);
							scale.y() = dataAccessor->getElementsComponentAsFloat(i, 1);
							scale.z() = dataAccessor->getElementsComponentAsFloat(i, 2);
							animationChannel->setFrame(i, time, scale);
						}
						animationChannelBase = animationChannel;
					}
					else if (animationTargetPath == "weights")
					{
						auto animationChannel = new mitevox::AnimationChannel<safety::SafeFloatArray>(
							&node->mesh->weights, interpolationType, animationFramesCount);
						for (size_t i = 0; i < animationFramesCount; ++i)
						{
							float time = timeAccessor->getElementsComponentAsFloat(i, 0);
							size_t morphTargetsCount = node->mesh->getMorphTargetsCount();
							safety::SafeFloatArray weights(morphTargetsCount);
							dataAccessor->getElementsComponentsAsFloatArray(
								&weights, 
								i, 
								morphTargetsCount);
							animationChannel->setFrame(i, time, weights);
						}
						animationChannelBase = animationChannel;
					}
					else
					{
						// TODO: log error
					}

					animationResult->addChannel(animationChannelBase);
				}
			}
		}
	}
}