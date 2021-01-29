
#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <unordered_map>

namespace mathem
{
	const float signesX[8] = { -1.f, 1.f, -1.f, 1.f, -1.f, 1.f, -1.f, 1.f };
	const float signesY[8] = { -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f, 1.f };
	const float signesZ[8] = { -1.f, -1.f, -1.f, -1.f, 1.f, 1.f, 1.f, 1.f };

	typedef unsigned long IDtype;


	template <typename T>
	class OctreeNode;


	template <typename T>
	class DataPoint
	{
		IDtype ID = 0;
		
	public:

		mathem::Point3D pos;
		T data;

		OctreeNode<T>* node = nullptr;

		DataPoint(IDtype _ID)
		{
			ID = _ID;
		}
		DataPoint(IDtype _ID, mathem::Point3D _pos, T _data)
		{
			ID = _ID;
			pos = _pos;
			data = _data;
		}
	};


	template <typename T>
	class OctreeNode
	{

	public:

		OctreeNode<T>* subNodes[8];
		mathem::Point3D origin;
		float halfSize;
		bool isLeaf;

		std::vector<DataPoint<T>*> dataPoints;

		OctreeNode(mathem::Point3D _origin, float _halfSize)
		{
			init(_origin, _halfSize);
		}
		~OctreeNode()
		{
			wipe();
		}

		void init(mathem::Point3D _origin, float _halfSize)
		{
			origin = _origin;
			halfSize = _halfSize;

			subNodes[8] = { nullptr };
			isLeaf = true;
		}
		void wipe()
		{
			if (!isLeaf)
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
				for (DataPoint<T>* point : dataPoints)
				{
					delete point;
				}
				dataPoints.clear();
			}
		}

		/// <summary>
		/// Creates 8 subnodes and moves the data to them sorting by position.
		/// </summary>
		void subdivide()
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
				DataPoint<T>* point = dataPoints[i];
				OctreeNode<T>* octant = suggestSubnode(point);
				octant->emplace(point);
			}
			dataPoints.clear();
		}

		/// <summary>
		/// Recursively moves data from the subnodes to the node and deletes the subnodes.
		/// </summary>
		void collapse()
		{
			for (size_t i = 0; i < 8; i++)
			{
				subNodes[i]->collapse();


				DataPoint<T>* tmpPoint = nullptr;
				for (size_t j = 0; j < subNodes[i]->dataPoints.size(); j++)
				{
					tmpPoint = subNodes[i]->dataPoints[j];
					tmpPoint->node = this;
					dataPoints.push_back(tmpPoint);
				}

				delete subNodes[i];
				subNodes[i] = nullptr;
			}
			isLeaf = true;
		}

		/// <summary>
		/// Recursively checks the node and subnodes if they have any DataPoints.
		/// </summary>
		bool empty()
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
					if (!subNodes[i]->empty())
					{
						return false;
					}
				}
				return true;
			}
		}

		/// <summary>
		/// Checks the subnodes are leaves and if they have any DataPoints.
		/// </summary>
		/// <returns></returns>
		bool shouldCollapse()
		{
			if (!isLeaf)
			{
				for (size_t i = 0; i < 8; i++)
				{
					if (!subNodes[i]->isLeaf || 
						(subNodes[i]->isLeaf && (subNodes[i]->dataPoints.size() != 0)))
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

		OctreeNode<T>* suggestSubnode(DataPoint<T>* point)
		{
			// Choose where to emplace based on position.
			int octant = point->pos.compareWith(origin);
			return subNodes[octant];
		}

		void emplace(DataPoint<T>* point)
		{
			if (isLeaf)
			{
				point->node = this;
				dataPoints.push_back(point);
			}
			else
			{	
				suggestSubnode(point)->emplace(point);
			}
		}
		bool remove(IDtype ID)
		{
			if (isLeaf)
			{
				for (size_t i = 0; i < dataPoints.size(); i++)
				{
					if (dataPoints[i]->ID == ID)
					{
						dataPoints[i]->node = nullptr;
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
		DataPoint<T>* get(IDtype ID)
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
					DataPoint<T>* tmpPoint = subNodes[i]->get(ID);
					if (tmpPoint)
					{
						return tmpPoint;
					}
				}
			}
			return nullptr;
		}

		/// <summary>
		/// Cuts off empty leaves, subdevides overloaded leaves.
		/// </summary>
		/// <param name="nodeCapacity"> - max amount of DataPoints per node.</param>
		void update(size_t nodeCapacity)
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
	};


	template <typename T>
	class Octree
	{
		mathem::UniqueIDGenerator<IDtype> IDGenerator;

		// Stores DataPoint pointers for quick access.
		std::unordered_map<IDtype, DataPoint<T>*> dataPoints;

		// Max amount of DataPoints per node.
		size_t nodeCapacity;

	public:

		OctreeNode<T>* origin;

		Octree(float halfSize, size_t _nodeCapacity)
		{
			init(halfSize, _nodeCapacity);
		}
		~Octree()
		{
			wipe();
		}

		void init(float halfSize, size_t _nodeCapacity)
		{
			origin = new OctreeNode<T>(mathem::Point3D(), halfSize);
			nodeCapacity = _nodeCapacity;
		}
		void wipe()
		{
			dataPoints.clear();
			delete origin;
			IDGenerator.reset();
		}

		OctreeNode<T>* getNearestNode(mathem::Point3D point)
		{
			if (isLeaf)
			{
				return this;
				dataPoints.push_back(point);
			}
			else
			{
				return suggestSubnode(point)->getNearestNode(point);
			}
		}

		IDtype emplace(mathem::Point3D pos, T value)
		{
			IDtype newID = IDGenerator.getID();
			DataPoint<T>* point = new DataPoint<T>(newID, pos, value);
			dataPoints[newID] = point;

			origin->emplace(point);
		}
		void remove(IDtype ID)
		{
			DataPoint<T>* point = dataPoints[ID];

			OctreeNode<T>* tmpNode = point->node;
			tmpNode->remove(ID);
			dataPoints.erase(ID);

			IDGenerator.returnID(ID);
		}

		/// <summary>
		/// Cuts off empty leaves, subdevides overloaded leaves.
		/// </summary>
		void update()
		{
			origin->update(nodeCapacity);
		}
	};
}

#endif
