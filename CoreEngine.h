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

#define type_assert(base, derived, msg) static_assert(std::is_base_of<base, derived>::value, msg)

namespace Engine
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

	class Component;

	/*
	*	Generic object class, for any object the engine might use
	*/
	class Object : public std::enable_shared_from_this<Object>
	{
	protected:
		bool bShouldTick = true;

		std::vector<Ownership<Component>> Components;
	public:
		inline Object() = default;
		virtual ~Object() = default;

		virtual void tick(float DeltaTime);

		virtual Pointer<Object> GetSelf() final;

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

	inline Pointer<Object> Object::GetSelf() { return shared_from_this(); }

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
		virtual ~Actor() override = default;
	};

	// --------------------------------------------------
	// COMPONENT FRAMEWORK
	// --------------------------------------------------

	/*
	*	A component is a special object that is made to be attached to other objects.
	*	Only its parent object can have an explicit pointer to it, this means that it's the
	*	object's duty to call the tick functions of the components.
	* 
	*	Components are very strict, only the parent object can have a unique, concrete
	*	pointer to them and they are considered part of the parent object themselves,
	*	therefore they are not counted in the engine's gameobjects collection.
	*/
	class Component : public Object
	{ 
	friend class Object;
	private:
		// This function is meant to be only called by Object in AddComponent and nowhere else
		virtual void Attach(const Pointer<Object>&) = 0;

	public:
		virtual ~Component() = default;
	};

	class ObjectComponent : public Component
	{
	private:
		Reference<Object> Parent;

		virtual void Attach(const Pointer<Object>&) override;
	public:
		virtual ~ObjectComponent() override = default;
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