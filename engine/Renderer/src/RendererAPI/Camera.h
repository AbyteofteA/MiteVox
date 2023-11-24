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
		float aspectRatio = 1.777777f;
		float nearCullPlane = 0.01f;
		float farCullPlane = 1500.0f;
		mathem::Vector3D upDirection = { 0.0f, 1.0f, 0.0f };

		Camera();
		Camera(float _FOV, int _width, int _height, float _nearCullPlane = 0.01f, float _farCullPlane = 1500.0f);
		void updateMatrices(mathem::GeometryTransform* cameraTransform);
		glm::mat4 makeViewMatrix(mathem::GeometryTransform* cameraTransform);
		glm::mat4 makeViewOrientationMatrix(mathem::GeometryTransform* cameraTransform);
		glm::mat4 getViewMatrix();
		glm::mat4 getViewOrientationMatrix();
		glm::mat4 getProjectionMatrix();

	private:

		glm::mat4 viewMatrix;
		glm::mat4 viewOrientationMatrix;
		glm::mat4 projectionMatrix;
	};
}

#endif