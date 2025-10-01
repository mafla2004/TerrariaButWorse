#ifndef __CORE_ENGINE__
#define __CORE_ENGINE__

#include "LinearAlgebra.h"
#include "MathFuncs.h"
#include "physics.h"
#include <unordered_map>
#include <cinttypes>
#include <numbers>	//#include <wrench> ;)
#include <memory>
#include <vector>
#include <string>
#include <map>

class Engine
{
/*	I planned to make a custom garbage collector, but since this is a learning project I decided
	to use C++'s smart pointers. I still plan to implement a custom garbage collector
	at some point though, so I use the preprocessor to "save" what was done as of now... */
#ifndef USE_CUSTOM_GARBAGE_COLLECTION
	template <class C>
	using Pointer = std::shared_ptr<C>;

	template<class C>
	using Reference = std::weak_ptr<C>;

	template<class C>
	using Ownership = std::unique_ptr<C>;

	template<class C>
	inline Pointer<C> MakePointer() { return std::make_shared<C>(); };

	template<class C>
	Pointer<C> MakePointer(size_t n) { return std::make_shared<C>(n); };

	template<class C, class... Args>
	Pointer<C> MakePointer(Args&&... args) { return std::make_shared(args); };
#else
	template <class C>
	class Pointer;

	template <class C>
	class Reference;

	template <class C>
	class Ownership;

	template<class C>
	Pointer<C> MakePointer();

	template<class C>
	Pointer<C> MakePointer(size_t n);

	template<class C, class... Args>
	Pointer<C> MakePointer(Args&&... args);
#endif

	// --------------------------------------------------
	// ENGINE BASE CLASSES
	// --------------------------------------------------

	/*
	*	Generic object class, for any object the engine might use
	*/
	class Object
	{
	protected:
		bool bShouldTick = true;

		std::vector<Ownership<class Component>> components;
	public:
		inline Object() = default;
		virtual ~Object() = default;

		virtual void tick(float DeltaTime) = 0;

		// Checks if the class already owns a component of a certain class
		template<class C>
		bool HasComponentOfClass();

		// Tries to add a component of a certain class, will fail if there is already a component
		// of the same class attached to the object
		template<class C>
		bool AddComponentOfClass();

		// Removes a component of a certain class
		template<class C>
		void RemoveComponentOfClass();

		// Returns a reference (weak pointer) to a component of a certain class 
		// attached to the object, null if there's none or if the component has been
		// removed
		template<class C>
		Reference<C> GetComponentOfClass();
	};

	/*
	*  An actor is an object that can be spawned in the game world
	*/
	class Actor : public Object
	{
	private:
		Vector2 Position;
		Vector2 Velocity;
		double Rotation;	// In radians

		Reference<Actor> Parent;
		std::vector<Pointer<Actor>> Children;
	public:
		virtual ~Actor() = default;
	};

	// --------------------------------------------------
	// COMPONENT FRAMEWORK
	// --------------------------------------------------

	/*
	*	A component is a special object that is made to be attached to other objects.
	*	Only its parent object can have an explicit pointer to it, this means that it's the
	*	object's duty to call the tick functions of the components.
	*/
	class Component : public Object
	{
		Reference<Object> Parent;
	};

#ifdef USE_CUSTOM_GARBAGE_COLLECTION
	// --------------------------------------------------
	// CUSTOM GARBAGE COLLECTION
	// --------------------------------------------------

	std::unordered_map<Object*, uint32_t> pointers;

	template<class C>
	class Pointer
	{
	private:
		C* raw;

		Pointer();
	public:
		virtual ~Pointer() = default;

		C* operator->() const noexcept;
	};

	template<class C>
	inline C* Pointer<C>::operator->() const noexcept { return raw; }
#endif
};

#endif