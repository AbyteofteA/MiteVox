#include "Node.h"

namespace mitevox
{
	bool Node::isAnimatedMesh()
	{
		if (mesh != nullptr)
		{
			if (mesh->isMorphable())
			{
				return true;
			}
			if (0/*TODO: skinIndex*/)
			{
				return true;
			}
		}
		return false;
	}

	Mesh* Node::getMeshToRender()
	{
		if (mesh != nullptr)
		{
			if (isAnimatedMesh())
			{
				return meshAnimationTarget;
			}
			else
			{
				return mesh;
			}
		}
		return nullptr;
	}
}