#ifndef __CORE_ENGINE__
#define __CORE_ENGINE__

#include "LinearAlgebra.h"
#include "MathFuncs.h"
#include "physics.h"
#include <type_traits>
#include <cinttypes>
#include <typeindex>
#include <typeinfo>
#include <numbers>	//#include <wrench> ;)
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <list>

namespace engine
{
	using namespace std;

	/*	This is a wrapper of C++'s shared_ptr, it is made to introduce automatic garbage collection.
		When there are no pointers pointing to the object, pointer<T> will immediately call the
		destructor. Keep in mind this is an owning reference, which can create cycles in
		parent-child object relationships, so in general, if there's a hierarchy of objects
		and you need a pointer to an object hugher up in the hierarchy, use reference.
	*/
	template<class T>
	using pointer = shared_ptr<T>;

	// Function wrappers made to better alias between pointer and shared_ptr

	// Makes a pointer to an object of type T using the default constructor if present
	template<class T>
	inline pointer<T> MakePointer() { return make_shared<T>(); }

	// Makes a pointer to an array of objects of type T
	template<class T>
	inline pointer<T> MakePointer(size_t n) { return make_shared<T>(n); }

	// Makes a pointer to an object of type T using a defined constructor and taking the arguments during the function call
	template<class T, typename... Args>
	inline pointer<T> MakePointer(Args&&... args) { return make_shared<T>(args); }

	/*	This is needed because of circular references:
		if an object is owned by another object and needs a reference to its owner
		for quick access, using pointer will cause a cycle and prevent the destruction
		of either of the two (because they will point at each other and 
		their pointer count willalways be >= 1), reference breaks this cycle by implementing
		a non-owning reference. */
	template<class T>
	using reference = weak_ptr<T>;

	template<class T>
	using ownership = unique_ptr<T>;

	class component;

	// --------------------------------------------------
	// BASE CLASSES
	// --------------------------------------------------

	class object : public enable_shared_from_this<object>
	{
	private:
		list<pointer<component>> components;	// Only one component per class
		list<string> tags;

	protected:
		bool bShouldTick = true;

	public:
		inline object() {}
		virtual ~object();

		virtual void tick(float DeltaTime) {}	// Would make it abstract but the compiler doesn't like that

		inline virtual pointer<object> getSelf() final { return shared_from_this(); }

		virtual bool HasTag(const string&)		final;
		virtual bool AddTag(const string&)		final;
		virtual void RemoveTag(const string&)	final;

		inline virtual uint32_t TagsAmount() final { return tags.size(); }

		inline virtual bool ShouldTick() final { return bShouldTick; }

		/*
		virtual bool HasComponent(const pointer<component>&)		final;
		virtual bool AddComponent(const pointer<component>&)		final;
		virtual bool RemoveComponent(const pointer<component>&)		final;
		*/

		template<class C>
		inline bool HasComponentOfClass() 
		{
			static_assert(is_base_of<component, C>::value, "Class must be derived from component");

			for (pointer<component> ptr : components)
			{
				if (dynamic_cast<C*>(ptr.get()))
				{
					return true;
				}
			}
			return false;
		}

		template<class C>
		inline bool AddComponentOfClass()
		{
			static_assert(is_base_of<component, C>::value, "Class must be derived from component");

			if (HasComponentOfClass<C>())
			{
				return false;
			}

			pointer<C> comp = make_shared<C>();

			if (!comp->AttachTo(getSelf()))
			{
				comp.reset();
				return false;
			}

			components.push_front(comp);
			return true;
		}

		// Version of the method with arguments
		// Args&& is a universal reference, meaning it can be a reference to rvalues as well
		template<class C, typename... Args>
		inline bool AddComponentOfClass(Args&&... args)
		{
			static_assert(is_base_of<component, C>::value, "Class must be derived from component");

			if (HasComponentOfClass<C>())
			{
				return false;
			}

			pointer<C> comp = make_shared<C>(forward<Args>(args));

			if (!comp->AttachTo(getSelf()))
			{
				comp.reset();
				return false;
			}

			components.push_front(comp);
			return true;
		}

		template<class C>
		inline reference<C> GetComponentOfClass()
		{
			static_assert(is_base_of<component, C>::value, "Class must be derived from component");

			for (pointer<component> ptr : components)
			{
				if (dynamic_cast<C*>(ptr.get()))
				{
					return ptr;
				}
			}

			return nullptr;
		}

		template<class C>
		inline void RemoveComponentOfClass()
		{
			static_assert(is_base_of<component, C>::value, "Class must be derived from component");
			
			for (pointer<component> ptr : components)
			{
				if (dynamic_cast<C*>(ptr.get()))
				{
					components.remove(ptr);
					return;
				}
			}
		}
	};

	inline void object::RemoveTag(const string& tag) { tags.remove(tag); }

	/*
	*	An actor represents a physical object that is present in the world,
	*	in its most basic informations, it has a position, a velocity, a parent actor
	*	and a sequence of child actors
	*/
	class actor : public object
	{
	private:
		Vector2 position;
		Vector2 velocity;
		double rotation;	// Expressed in radians

		reference<actor> parent;

		list<pointer<actor>> children;

		inline virtual void DetachChild(const pointer<actor>& child) final { children.remove(child); }
	public:
		inline actor(const Vector2& init_pos = ZERO2D, const Vector2& init_vel = ZERO2D, double rot = 0.0) :
			position(init_pos), velocity(init_vel), rotation(rot) {}

		virtual ~actor() = default;

		inline virtual void tick(float DeltaTime) override { object::tick(DeltaTime); position += velocity * DeltaTime; }
	};

	// --------------------------------------------------
	// COMPONENT FRAMEWORK
	// --------------------------------------------------

	/*
	*	Components are classes that can be attached to any object to implement some extra modular
	*	logic. They are objects in and of themselves and as such they can have components too.
	*
	*	This is an abstract implementation of the component class.
	*/
	class component : public object
	{
	friend class object;	// Only object is supposed to be able to call these methods
	protected:
		virtual bool AttachTo(const pointer<object>&)		= 0;
		virtual bool IsAttachedTo(const pointer<object>&)	final;
		virtual void DetachFromParent()						= 0;

	public:
		virtual pointer<object> GetParent()			= 0;
	};

	inline bool component::IsAttachedTo(const pointer<object>& obj) { return GetParent() == obj; }

	/*
	*	Concretization of component compatible with both objects and actors
	*/
	class ObjectComponent : public component
	{
	private:
		reference<object> parent;

	protected:
		virtual bool AttachTo(const pointer<object>&)	override;
		virtual void DetachFromParent()					override;

	public:
		inline virtual pointer<object> GetParent() override final { return parent.lock(); }
	};

	/*
	*	Concretization of component made to be attached to actors alone
	*/
	class ActorComponent : public component
	{
	private:
		reference<actor> parent;

	protected:
		virtual bool AttachTo(const pointer<object>& obj) override;

	public:
		inline virtual pointer<object>	GetParent() override final { return parent.lock(); }
		inline virtual pointer<actor>	GetParentAsActor() final { return parent.lock(); }
	};

	class SceneComponent : public ActorComponent
	{
	private:
		Vector2 RelativePosition;
		Vector2 RelativeVelocity;
		double  RelativeRotation;	// expressed in radians

	public:
		inline SceneComponent(	const Vector2& init_pos = ZERO2D,
								const Vector2& init_vel = ZERO2D,
								double rot = 0.0) 
								: RelativePosition(init_pos), 
								  RelativeVelocity(init_vel), 
								  RelativeRotation(rot) {}
	};

	// --------------------------------------------------
	// GAME WORLD
	// --------------------------------------------------

	/*
	*	A world class that is organized in a grid.
	*	This is useful for quick collision detections.
	*	No implementative details yet, a lot has to be established.
	*/
	class world
	{

	};
}

#endif