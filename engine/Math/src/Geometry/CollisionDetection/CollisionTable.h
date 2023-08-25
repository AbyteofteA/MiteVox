#ifndef COLLISIONTABLE_H
#define COLLISIONTABLE_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"

#include <mutex>

namespace mathem
{
	typedef CollisionType(*CollisionChecker)(GeometryPrimitiveBase*, GeometryTransform*, GeometryPrimitiveBase*, GeometryTransform*, CollisionProperties*);

	class CollisionTable
	{
	public:

		static void tryInit();

		static CollisionType checkCollision(
			GeometryPrimitiveBase* geometryPrimitive1,
			GeometryTransform* geometryPrimitive1Transform,
			GeometryPrimitiveBase* geometryPrimitive2,
			GeometryTransform* geometryPrimitive2Transform,
			CollisionProperties* collisionProperties);

		CollisionTable(CollisionTable& other) = delete;
		void operator=(const CollisionTable&) = delete;

	private:

		static bool needToInit;
		static std::mutex mutex;
		static CollisionChecker collisionTable[(size_t)GeometryPrimitiveType::count][(size_t)GeometryPrimitiveType::count];

		CollisionTable();
		~CollisionTable();
	};
}

#endif
