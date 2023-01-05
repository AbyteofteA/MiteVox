#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <string>
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/gtc/matrix_transform.hpp"
#include "dependencies/glm/gtc/type_ptr.hpp"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

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
		int width = 0, height = 0;
		float nearCullPlane = 0.1f;
		float farCullPlane = 100000.f;
		mathem::Vector3D upDirection = { 0.0f, 1.0f, 0.0f };

		Camera();
		Camera(float _FOV, int _width, int _height, float _nearCullPlane = 0.1f, float _farCullPlane = 10000.f);
		glm::mat4 getViewProjectionMatrix(mathem::GeometryTransform* cameraTransform);
	};
}

#endif