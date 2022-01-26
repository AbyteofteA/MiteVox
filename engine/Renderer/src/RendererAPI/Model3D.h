
#ifndef RENDERER_MODEL3D_H
#define RENDERER_MODEL3D_H

namespace render
{
	class Model3D
	{
	public:
		Mesh3D* mesh = nullptr;
		fileio::Material* material = nullptr;
		mathem::Transform transform;
		Cubemap* reflectionMap = nullptr;

		int shaderID = 0;

		Model3D(Mesh3D* _mesh, fileio::Material* _material,
				mathem::Transform _transform = mathem::Transform())
		{
			mesh = _mesh;
			material = _material;
			transform = _transform;
			reflectionMap = nullptr;
		}
	};
}

#endif