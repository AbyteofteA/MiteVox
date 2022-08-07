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
		animationResult->_name = animationJSON->getFieldString("name");

		JSON* animationSamplersArrayJSON = animationJSON->getFieldArray("samplers");
		size_t animationSamplersCount = 0;
		if (animationSamplersArrayJSON != nullptr)
		{
			animationSamplersCount = animationSamplersArrayJSON->getArraySize();
		}

		JSON* animationChannelsArrayJSON = animationJSON->getFieldArray("channels");
		if (animationChannelsArrayJSON != nullptr)
		{
			size_t animationChannelsCount = animationChannelsArrayJSON->getArraySize();
			// TODO: reserve animationChannels

			for (size_t i = 0; i < animationChannelsCount; ++i)
			{
				JSON* animationChannelJSON = animationChannelsArrayJSON->getArrayItem(i);

				int32_t samplerIndex = -1;
				JSON* numberJSON = animationChannelJSON->getField("sampler");
				if ((numberJSON != nullptr) && (numberJSON->isNumber()))
				{
					samplerIndex = (int32_t)numberJSON->getNumber();
				}
				mitevox::BufferViewAccessor* timeAccessor = nullptr;
				mitevox::BufferViewAccessor* dataAccessor = nullptr;
				mitevox::InterpolationType interpolationType = mitevox::InterpolationType::LINEAR;
				size_t animationFramesCount = 0;
				JSON* animationSamplerJSON = animationSamplersArrayJSON->getArrayItem(samplerIndex);
				if (animationSamplerJSON != nullptr)
				{
					numberJSON = animationSamplerJSON->getField("input");
					if ((numberJSON != nullptr) && (numberJSON->isNumber()))
					{
						timeAccessor = accessors->getElement((size_t)numberJSON->getNumber());
					}

					numberJSON = animationSamplerJSON->getField("output");
					if ((numberJSON != nullptr) && (numberJSON->isNumber()))
					{
						dataAccessor = accessors->getElement((size_t)numberJSON->getNumber());
					}

					animationFramesCount = timeAccessor->count;

					std::string interpolationTypeString = "LINEAR";
					if (JSON* interpolationTypeJSON = animationSamplerJSON->getField("interpolation"))
					{
						interpolationTypeString = interpolationTypeJSON->getString();
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
				std::string animationTargetPath = animationTargetJSON->getField("path")->getString();
				numberJSON = animationTargetJSON->getField("node");
				if ((numberJSON != nullptr) && (numberJSON->isNumber()))
				{
					if (animationTargetPath == "translation")
					{
						mitevox::Node* node = nodes->getElement((size_t)numberJSON->getNumber());
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
						mitevox::Node* node = nodes->getElement((size_t)numberJSON->getNumber());
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
						mitevox::Node* node = nodes->getElement((size_t)numberJSON->getNumber());
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
						mitevox::Node* node = nodes->getElement((size_t)numberJSON->getNumber());
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
						// TODO: error
					}

					animationResult->addChannel(animationChannelBase);
				}
			}
		}
	}
}