#include "Node.h"

namespace mitevox
{
	bool Node::isMorphableMesh()
	{
		if (mesh != nullptr)
		{
			if (mesh->isMorphable())
			{
				return true;
			}
		}
		return false;
	}

	Mesh* Node::getMeshToRender()
	{
		if (isMorphableMesh())
		{
			return morphAnimationTarget;
		}
		return mesh;
	}
}