#ifndef NODEBASEDSKELETONCODEC_H
#define NODEBASEDSKELETONCODEC_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Skeleton/NodeBasedSkeleton.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace fileio
{
	class NodeBasedSkeletonCodec
	{
    public:

        static void fromGLTF(
            mitevox::NodeBasedSkeleton* skeletonResult,
            JSON* skeletonJSON,
            safety::SafeArray<mitevox::Node*>* nodes,
            safety::SafeArray<mitevox::BufferViewAccessor*>* accessors);

		static void toGLTF(
			JSON* skeletonJSONResult,
			mitevox::NodeBasedSkeleton* skeleton,
			safety::SafeArray<mitevox::Node*>* nodes,
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors);
	};
}

#endif

