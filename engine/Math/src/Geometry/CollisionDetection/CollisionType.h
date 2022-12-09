#ifndef COLLISIONTYPE_H
#define COLLISIONTYPE_H

#include <cstdint>

namespace mathem
{
	enum struct CollisionType : uint8_t 
	{ 
		NONE, 
		INTERSECTION, 
		INSCRIBTION_1_2, 
		INSCRIBTION_2_1 
	};
}

#endif