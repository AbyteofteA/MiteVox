
#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <string>

namespace render
{
	enum struct CameraType
	{
		INVALID = -1,
		PERPECTIVE,
		ORTHOGRAPHIC
	};

	class Camera
	{
	public:

		CameraType _type = CameraType::PERPECTIVE;
		std::string _name;
		float FOV = 45.f;
		int width, height;
		float nearCullPlane = 0.1f;
		float farCullPlane = 100000.f;

		Camera() {}

		Camera(float _FOV, int _width, int _height, float _nearCullPlane = 0.1f, float _farCullPlane = 10000.f)
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