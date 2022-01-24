
#ifndef OCTREENODE_H
#define OCTREENODE_H

#include "OctreeDataPoint.h"
#include "engine/Math/src/LinearAlgebra/Point3D.h"

#include <vector>

namespace mathem
{

	const float signesX[8] = { -1.f, 1.f, -1.f, 1.f, -1.f, 1.f, -1.f, 1.f };
	const float signesY[8] = { -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f, 1.f };
	const float signesZ[8] = { -1.f, -1.f, -1.f, -1.f, 1.f, 1.f, 1.f, 1.f };


	template <typename T>
	class OctreeNode
	{
		template <typename T>
		friend class Octree;

	public:

		OctreeNode<T>* subNodes[8];
		mathem::Point3D origin;
		float halfSize;
		bool isLeaf;

		std::vector<OctreeDataPoint<T>*> dataPoints;

		OctreeNode(mathem::Point3D _origin, float _halfSize);
		~OctreeNode();

	private:

		/// <summary>
		/// Creates 8 subnodes and moves the data to them sorting by position.
		/// </summary>
		void subdivide();

		/// <summary>
		/// Recursively moves data from the subnodes to the node and deletes the subnodes.
		/// </summary>
		void collapse();

		/// <summary>
		/// Recursively checks the node and subnodes if they have any OctreeDataPoints.
		/// </summary>
		bool isEmpty();

		/// <summary>
		/// Checks the subnodes are leaves and if they have any OctreeDataPoints.
		/// </summary>
		/// <returns></returns>
		bool shouldCollapse();

		OctreeNode<T>* suggestSubnode(OctreeDataPoint<T>* point);

		void emplace(OctreeDataPoint<T>* point);
		bool remove(IDtype ID);
		OctreeDataPoint<T>* get(IDtype ID);

		/// <summary>
		/// Cuts off empty leaves, subdevides overloaded leaves.
		/// </summary>
		/// <param name="nodeCapacity"> - max amount of OctreeDataPoints per node.</param>
		void update(size_t nodeCapacity);
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	OctreeNode<T>::OctreeNode(mathem::Point3D _origin, float _halfSize)
	{
		origin = _origin;
		halfSize = _halfSize;

		subNodes[8] = { nullptr };
		isLeaf = true;
	}

	template <typename T>
	OctreeNode<T>::~OctreeNode()
	{
		if (not isLeaf)
		{
			for (size_t i = 0; i < 8; i++)
			{
				delete subNodes[i];
				subNodes[i] = nullptr;
			}
			isLeaf = true;
		}
		else
		{
			for (OctreeDataPoint<T>* point : dataPoints)
			{
				delete point;
			}
			dataPoints.clear();
		}
	}

	template <typename T>
	void OctreeNode<T>::subdivide()
	{
		float newSize = halfSize / 2.0;

		// Create subnodes (octants).
		for (size_t i = 0; i < 8; i++)
		{
			mathem::Point3D newOrigin;
			newOrigin.x = origin.x + newSize * signesX[i];
			newOrigin.y = origin.y + newSize * signesY[i];
			newOrigin.z = origin.z + newSize * signesZ[i];
			subNodes[i] = new OctreeNode<T>(newOrigin, halfSize / 2.0);
		}
		isLeaf = false;

		// Move the data to the subnodes.
		for (size_t i = 0; i < dataPoints.size(); i++)
		{
			OctreeDataPoint<T>* point = dataPoints[i];
			OctreeNode<T>* octant = suggestSubnode(point);
			octant->emplace(point);
		}
		dataPoints.clear();
	}

	template <typename T>
	void OctreeNode<T>::collapse()
	{
		for (size_t i = 0; i < 8; i++)
		{
			subNodes[i]->collapse();


			OctreeDataPoint<T>* tmpPoint = nullptr;
			for (size_t j = 0; j < subNodes[i]->dataPoints.size(); j++)
			{
				tmpPoint = subNodes[i]->dataPoints[j];
				tmpPoint->_parentOctreeNode = this;
				dataPoints.push_back(tmpPoint);
			}

			delete subNodes[i];
			subNodes[i] = nullptr;
		}
		isLeaf = true;
	}

	template <typename T>
	bool OctreeNode<T>::isEmpty()
	{
		if (isLeaf)
		{
			if (dataPoints.size() == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			for (size_t i = 0; i < 8; i++)
			{
				if (not subNodes[i]->isEmpty())
				{
					return false;
				}
			}
			return true;
		}
	}

	template <typename T>
	bool OctreeNode<T>::shouldCollapse()
	{
		if (not isLeaf)
		{
			for (size_t i = 0; i < 8; i++)
			{
				if (not subNodes[i]->isLeaf || (subNodes[i]->isLeaf && (subNodes[i]->dataPoints.size() != 0)))
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	OctreeNode<T>* OctreeNode<T>::suggestSubnode(OctreeDataPoint<T>* point)
	{
		// Choose where to emplace based on position.
		int octant = point->pos.compareWith(origin);
		return subNodes[octant];
	}

	template <typename T>
	void OctreeNode<T>::emplace(OctreeDataPoint<T>* point)
	{
		if (isLeaf)
		{
			point->_parentOctreeNode = this;
			dataPoints.push_back(point);
		}
		else
		{
			suggestSubnode(point)->emplace(point);
		}
	}

	template <typename T>
	bool OctreeNode<T>::remove(IDtype ID)
	{
		if (isLeaf)
		{
			for (size_t i = 0; i < dataPoints.size(); i++)
			{
				if (dataPoints[i]->ID == ID)
				{
					dataPoints[i]->_parentOctreeNode = nullptr;
					dataPoints.erase(dataPoints.begin() + i);
					return true;
				}
			}
		}
		else
		{
			for (size_t i = 0; i < 8; i++)
			{
				if (subNodes[i]->remove(ID))
				{
					return true;
				}
			}
		}
		return false;
	}

	template <typename T>
	OctreeDataPoint<T>* OctreeNode<T>::get(IDtype ID)
	{
		if (isLeaf)
		{
			for (size_t i = 0; i < dataPoints.size(); i++)
			{
				if (dataPoints[i]->ID == ID)
				{
					return dataPoints[i];
				}
			}
		}
		else
		{
			for (size_t i = 0; i < 8; i++)
			{
				OctreeDataPoint<T>* tmpPoint = subNodes[i]->get(ID);
				if (tmpPoint)
				{
					return tmpPoint;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	void OctreeNode<T>::update(size_t nodeCapacity)
	{
		if (isLeaf)
		{
			if (dataPoints.size() >= nodeCapacity)
			{
				subdivide();
				for (size_t i = 0; i < 8; i++)
				{
					subNodes[i]->update();
				}
			}
		}
		else
		{
			if (shouldCollapse())
			{
				collapse();
			}
			else
			{
				for (size_t i = 0; i < 8; i++)
				{
					subNodes[i]->update();
				}
			}
		}
	}
}

#endif