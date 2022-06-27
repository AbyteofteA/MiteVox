
#ifndef MATH_TRANSFORM_H
#define MATH_TRANSFORM_H

namespace mathem
{
	struct Transform
	{
		float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
		float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
		float x = 0.0f, y = 0.0f, z = 0.0f;

		Transform(
			float _scaleX = 1.0f, float _scaleY = 1.0f, float _scaleZ = 1.0f,
			float _angleX = 0.0f, float _angleY = 0.0f, float _angleZ = 0.0f,
			float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
		{
			scaleX = _scaleX;
			scaleY = _scaleY;
			scaleZ = _scaleZ;
			angleX = _angleX;
			angleY = _angleY;
			angleZ = _angleZ;
			x = _x;
			y = _y;
			z = _z;
		}

		mathem::Vector3D getDirectionVector()
		{
			mathem::Vector3D resultVector = { 0, 0, 1 };
			/*glm::vec3 vector = { 0, 0, 1 };
			glm::mat4 global = glm::mat4(1.0f);
			global = glm::rotate(global, glm::radians(angleX), glm::vec3(1.0, 0.0, 0.0));
			global = glm::rotate(global, glm::radians(angleY), glm::vec3(0.0, 1.0, 0.0));
			global = glm::rotate(global, glm::radians(angleZ), glm::vec3(0.0, 0.0, 1.0));
			vector = vector * glm::mat3(global);
			resultVector.i = vector.x;
			resultVector.j = vector.y;
			resultVector.k = vector.z;
			resultVector.normalize();*/

			resultVector = transformVector(resultVector);

			return resultVector;
		}

		Point3D transformPoint3D(Point3D v)
		{
			Point3D vNew = { v.x, v.y, v.z };
			Point3D vTmp = { v.x, v.y, v.z };

			float angle = angleY * (float)M_PI / 180;
			vTmp.x = (float)cos(angle) * vNew.x + (float)sin(angle) * vNew.z;
			vTmp.z = (float)-sin(angle) * vNew.x + (float)cos(angle) * vNew.z;
			vNew.x = vTmp.x;
			vNew.z = vTmp.z;

			angle = angleX * (float)M_PI / 180;
			vTmp.y = (float)cos(angle) * vNew.y + (float)sin(angle) * vNew.z;
			vTmp.z = (float)-sin(angle) * vNew.y + (float)cos(angle) * vNew.z;
			vNew.y = vTmp.y;
			vNew.z = vTmp.z;

			angle = angleZ * (float)M_PI / 180;
			vTmp.x = (float)cos(angle) * vNew.x - (float)sin(angle) * vNew.y;
			vTmp.y = (float)sin(angle) * vNew.x + (float)cos(angle) * vNew.y;
			vNew.x = vTmp.x;
			vNew.y = vTmp.y;

			// Scale
			vNew.x *= scaleX;
			vNew.y *= scaleY;
			vNew.z *= scaleZ;

			// Translate
			vNew.x += x;
			vNew.y += y;
			vNew.z += z;

			return vNew;
		}

		Vector3D transformVector(Vector3D v)
		{
			Vector3D vNew = v;
			Vector3D vTmp = v;


			float angle = angleY * (float)M_PI / 180;
			vTmp.i() = (float)cos(angle) * vNew.i() + (float)sin(angle) * vNew.k();
			vTmp.k() = (float)-sin(angle) * vNew.i() + (float)cos(angle) * vNew.k();
			vNew.i() = vTmp.i();
			vNew.k() = vTmp.k();

			angle = angleX * (float)M_PI / 180;
			vTmp.j() = (float)cos(angle) * vNew.j() - (float)sin(angle) * vNew.k();
			vTmp.k() = (float)sin(angle) * vNew.j() + (float)cos(angle) * vNew.k();
			vNew.j() = vTmp.j();
			vNew.k() = vTmp.k();

			angle = angleZ * (float)M_PI / 180;
			vTmp.i() = (float)cos(angle) * vNew.i() - (float)sin(angle) * vNew.j();
			vTmp.j() = (float)sin(angle) * vNew.i() + (float)cos(angle) * vNew.j();
			vNew.i() = vTmp.i();
			vNew.j() = vTmp.j();

			
			return vNew;
		}
	};


}

#endif