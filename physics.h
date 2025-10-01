#ifndef __PHYS__
#define __PHYS__

#include "EngineMemory.h"
#include <string>

namespace Engine
{
	constexpr double gravity = 9.81;

	// Forward Declarations
	class Actor;
	class Object;
	class Collider;

	/*	NOTE: String seems to be a rather memory inefficient type, consider changing it
		to other data types like a custom string or string_view */

	// Default collision channels
	const std::string NO_COLLISION			= "None";
	const std::string STATIC_COLLISION		= "Static";
	const std::string DYNAMIC_COLLISION		= "Dynamic";
	const std::string ENTITY_COLLISION		= "Entity";
	const std::string CHARACTER_COLLISION	= "Character";
	
	typedef struct
	{
		Pointer<Actor>		OtherActor;
		Pointer<Collider>	OtherCollider;
	} CollisionResults;
	
	/*
	*	Base Class for colliders
	*/
	class Collider
	{
	private:
		std::string CollisionChannel;

	public:
		inline Collider(const std::string& collision = NO_COLLISION) : CollisionChannel(collision) {}
		virtual ~Collider() = default;

		virtual std::string GetCollisionChannel() const final;
	};

	inline std::string Collider::GetCollisionChannel() const { return CollisionChannel; }
}

#endif