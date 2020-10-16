
#ifndef RENDERER_MODEL3D_H
#define RENDERER_MODEL3D_H

namespace render
{
	class Model3D
	{
	public:
		Mesh3D* model = nullptr;

		mathem::Transform* transform = nullptr;

		Material* material = nullptr;

		int shaderID = 0;

		Model3D(Mesh3D* m)
		{
			model = m;
			transform = new mathem::Transform();
			material = nullptr;
		}
		Model3D(Mesh3D* m, float scaleX, float scaleY, float scaleZ,
							float angleX, float angleY, float angleZ,
								float x, float y, float z)
		{
			model = m;
			transform = new mathem::Transform(scaleX, scaleY, scaleZ, angleX, angleY, angleZ, x, y, z);
			material = nullptr;
		}
	};
}

#endif