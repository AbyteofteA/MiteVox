
#ifndef MATH_TRANSFORM_H
#define MATH_TRANSFORM_H

namespace mathem
{
	struct Transform
	{
		float scaleX = 0.f, scaleY = 0.f, scaleZ = 0.f;
		float angleX = 0.f, angleY = 0.f, angleZ = 0.f;
		float x = 0.f, y = 0.f, z = 0.f;

		Transform(float _scaleX = 1, float _scaleY = 1, float _scaleZ = 1,
			float _angleX = 0, float _angleY = 0, float _angleZ = 0,
			float _x = 0, float _y = 0, float _z = 0)
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
			mathem::Vector3D resultVector = { 0, 0, -1 };

			resultVector = transformVector(resultVector);

			resultVector.normalize();

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
			Vector3D vNew = { v.i, v.j, v.k, v.length };
			Vector3D vTmp = { v.i, v.j, v.k, v.length };


			float angle = angleY * (float)M_PI / 180;
			vTmp.i = (float)cos(angle) * vNew.i + (float)sin(angle) * vNew.k;
			vTmp.k = (float)-sin(angle) * vNew.i + (float)cos(angle) * vNew.k;
			vNew.i = vTmp.i;
			vNew.k = vTmp.k;

			angle = angleX * (float)M_PI / 180;
			vTmp.j = (float)cos(angle) * vNew.j - (float)sin(angle) * vNew.k;
			vTmp.k = (float)sin(angle) * vNew.j + (float)cos(angle) * vNew.k;
			vNew.j = vTmp.j;
			vNew.k = vTmp.k;

			angle = angleZ * (float)M_PI / 180;
			vTmp.i = (float)cos(angle) * vNew.i - (float)sin(angle) * vNew.j;
			vTmp.j = (float)sin(angle) * vNew.i + (float)cos(angle) * vNew.j;
			vNew.i = vTmp.i;
			vNew.j = vTmp.j;

			
			return vNew;
		}
	};


}

#endif