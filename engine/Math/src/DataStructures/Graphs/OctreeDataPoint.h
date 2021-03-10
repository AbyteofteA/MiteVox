
#ifndef OCTREEDATAPOINT_H
#define OCTREEDATAPOINT_H

#include "engine/Math/src/LinearAlgebra/Point3D.h"

namespace mathem
{
	typedef unsigned long IDtype;


	template <typename T>
	class OctreeNode;


	template <typename T>
	class OctreeDataPoint
	{
		template <typename T>
		friend class OctreeNode;

	public:

		OctreeDataPoint(IDtype ID);
		OctreeDataPoint(IDtype ID, Point3D pos, T data);

		// Getters //

		Point3D getPosition();

	private:

		IDtype _ID = 0;
		Point3D _pos;

		OctreeNode<T>* _parentOctreeNode = nullptr;
		T _data;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	OctreeDataPoint<T>::OctreeDataPoint(IDtype ID)
	{
		_ID = ID;
	}

	template <typename T>
	OctreeDataPoint<T>::OctreeDataPoint(IDtype ID, Point3D pos, T data)
	{
		_ID = ID;
		_pos = pos;
		_data = data;
	}

	template <typename T>
	Point3D OctreeDataPoint<T>::getPosition()
	{
		return _pos;
	}
}

#endif