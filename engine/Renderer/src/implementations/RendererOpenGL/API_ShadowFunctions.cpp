#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include "engine/Math/src/Utilities/Convertations.h"

namespace render
{
	void renderMeshToShadowMap(
		RendererSettings* renderer,
		int shaderID,
		mitevox::Mesh* mesh,
		mathem::GeometryTransform* transform)
	{
		mathem::Matrix4x4 modelMatrix = mathem::transformToMatrix(*transform);
		shaders[shaderID]->setMatrix4x4("modelMatrix", modelMatrix);

		int64_t meshesPrimitivesCount = mesh->primitives.getElementsCount();
		for (int64_t primitiveIndex = 0; primitiveIndex < meshesPrimitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(primitiveIndex);

			renderer->amountOfDrawCalls++;

			glBindVertexArray(meshPrimitive->ID);

			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				glDrawElements(GL_TRIANGLES, indexAccessor->count, indexAccessor->componentType, (GLvoid*)indexAccessor->byteOffset);
			}
			else
			{
				size_t vertecesCount = meshPrimitive->attributes.byName.positionAccessor->count;
				glDrawArrays(GL_TRIANGLES, 0, vertecesCount);
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			PRINT_RENDERER_ERRORS;
		}
	}
}