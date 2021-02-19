
#ifndef ENTITY_H
#define ENTITY_H

namespace ecs
{
	/// <summary>
	/// Entity is just an unique ID + component flag.
	/// </summary>
	template <typename entityID>
	struct Entity
	{
		entityID ID = 0;
		entityID components = 0;
	};
}

#endif