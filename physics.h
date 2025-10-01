#ifndef __PHYS__
#define __PHYS__

#include <string>

namespace engine
{
	constexpr double gravity = 9.81;

	using namespace std;

	// Forward Declarations
	class actor;
	class object;
	template<class T>
	class pointer;

	/*	NOTE: String seems to be a rather memory inefficient type, consider changing it
		to other data types like a custom string or string_view */

	// Default collision channels
	const string NO_COLLISION		= "None";
	const string STATIC_COLLISION	= "Static";
	const string DYNAMIC_COLLISION	= "Dynamic";
	const string ENTITY_COLLISION	= "Entity";
	const string CHARACTER_COLLISION= "Character";

	typedef struct
	{
		pointer<actor>		OtherActor;
		pointer<collider>	OtherCollider;
	} CollisionResults;

	/*
	*	Base Class for colliders
	*/
	class collider
	{
	private:
		string CollisionChannel;

	public:
		inline collider(const string& collision = NO_COLLISION) : CollisionChannel(collision) {}
		virtual ~collider() = default;

		virtual string GetCollisionChannel() const final;
	};

	inline string collider::GetCollisionChannel() const { return CollisionChannel; }
}

#endif