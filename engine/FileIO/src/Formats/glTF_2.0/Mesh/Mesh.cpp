#include "Mesh.h"

namespace fileio
{
	Mesh::Mesh()
	{

	}

	void Mesh::fromGLTF(
		JSON* meshJSON,
		safety::SafeArray<BufferViewAccessor*>* accessors,
		safety::SafeArray<Material*>* materials)
	{
		name = meshJSON->getFieldString("name");

		JSON* weightsArrayJSON = meshJSON->getFieldArray("weights");
		if (weightsArrayJSON != nullptr)
		{
			weightsArrayJSON->toNumberArray<float>(&weights);
		}

		JSON* meshPrimitivesArrayJSON = meshJSON->getFieldArray("primitives");
		if (meshPrimitivesArrayJSON != nullptr)
		{
			size_t meshPrimitivesArraySize = meshPrimitivesArrayJSON->getArraySize();
			primitives.resize(meshPrimitivesArraySize);

			for (size_t i = 0; i < meshPrimitivesArraySize; ++i)
			{
				JSON* meshPrimitiveJSON = meshPrimitivesArrayJSON->getArrayItem(i);
				MeshPrimitive* meshPrimitive = new MeshPrimitive();
				meshPrimitive->fromGLTF(meshPrimitiveJSON, accessors, materials);
				primitives.setElement(i, meshPrimitive);
			}
		}
	}
}