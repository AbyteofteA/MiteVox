
#ifndef MATH_POINT3D_H
#define MATH_POINT3D_H

namespace mathem
{

	struct Point3D
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};


	//===================================//
   //		Point3D transformations		//
  //===================================//


	inline Point3D translate(Point3D v, float x, float y, float z)
	{
		Point3D vNew = { v.x, v.y, v.z };
		vNew.x += x;
		vNew.y += y;
		vNew.z += z;

		return vNew;
	}


	inline Point3D resize(Point3D v, float x, float y, float z)
	{
		Point3D vNew = { v.x, v.y, v.z };
		vNew.x *= x;
		vNew.y *= y;
		vNew.z *= z;

		return vNew;
	}


	inline Point3D rotateX(Point3D v, float angle)
	{
		angle *= (float)M_PI / 180;

		Point3D vNew;
		vNew.x = v.x;
		vNew.y = (float)cos(angle) * v.y - (float)sin(angle) * v.z;
		vNew.z = (float)sin(angle) * v.y + (float)cos(angle) * v.z;

		return vNew;
	}


	inline Point3D inverseRotateX(Point3D v, float angle)
	{
		angle *= (float)M_PI / 180;

		Point3D vNew;
		vNew.x = v.x;
		vNew.y = (float)cos(angle) * v.y + (float)sin(angle) * v.z;
		vNew.z = (float)-sin(angle) * v.y + (float)cos(angle) * v.z;

		return vNew;
	}


	inline Point3D rotateY(Point3D v, float angle)
	{
		angle *= (float)M_PI / 180;

		Point3D vNew;
		vNew.x = (float)cos(angle) * v.x - (float)sin(angle) * v.z;
		vNew.y = v.y;
		vNew.z = (float)sin(angle) * v.x + (float)cos(angle) * v.z;

		return vNew;
	}


	inline Point3D inverseRotateY(Point3D v, float angle)
	{
		angle *= (float)M_PI / 180;

		Point3D vNew;
		vNew.x = (float)cos(angle) * v.x + (float)sin(angle) * v.z,
			vNew.y = v.y;
		vNew.z = (float)-sin(angle) * v.x + (float)cos(angle) * v.z;

		return vNew;
	}


	inline Point3D rotateZ(Point3D v, float angle)
	{
		angle *= (float)M_PI / 180;

		Point3D vNew;
		vNew.x = (float)cos(angle) * v.x - (float)sin(angle) * v.y;
		vNew.y = (float)sin(angle) * v.x + (float)cos(angle) * v.y;
		vNew.z = v.z;

		return vNew;
	}


	inline Point3D inverseRotateZ(Point3D v, float angle)
	{
		angle *= (float)M_PI / 180;

		Point3D vNew;
		vNew.x = (float)cos(angle) * v.x + (float)sin(angle) * v.y;
		vNew.y = (float)-sin(angle) * v.x + (float)cos(angle) * v.y;
		vNew.z = v.z;

		return vNew;
	}

}

#endif