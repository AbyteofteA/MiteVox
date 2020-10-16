
#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

namespace render
{
	class Camera
	{
	public:

		float FOV = 45.f;
		int width, height;
		float nearCullPlane = 1.f;
		float farCullPlane = 100000.f;

		Camera(float _FOV, int _width, int _height, float _nearCullPlane = 1.f, float _farCullPlane = 10000.f)
		{
			FOV = _FOV;
			width = _width;
			height = _height;
			nearCullPlane = _nearCullPlane;
			farCullPlane = _farCullPlane;
		}
	};
}

#endif