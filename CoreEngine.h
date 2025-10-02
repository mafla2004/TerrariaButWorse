#ifndef __CORE_ENGINE__
#define __CORE_ENGINE__

#include "LinearAlgebra.h"
#include "EngineMemory.h"
#include "MathFuncs.h"
#include "physics.h"
#include <unordered_map>
#include <cinttypes>
#include <iostream>
#include <numbers>	//#include <wrench> ;)
#include <memory>
#include <vector>
#include <string>
#include <map>

#define type_assert(base, derived, msg) static_assert(std::is_base_of<base, derived>::value, msg)
#define debug_print(stream) std::cout << stream << std::endl

namespace Engine
{
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

		std::vector<Pointer<Component>> Components;
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

		// Tries to add a component of a class, using the parameters to call a specific constructor
		template<class C, class... Args>
		bool AddComponentOfClass(Args&&... args);

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

		virtual void tick(float DeltaTime) override;
	};

	inline void Actor::tick(float DeltaTime) 
	{ 
		Position += Velocity * DeltaTime; 
		Object::tick(DeltaTime); 
	}

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
		// Theses functions are meant to be only called by Object in AddComponent and nowhere else
		virtual bool Attach(const Pointer<Object>&) = 0;
		virtual void Detach() = 0;
	public:
		virtual ~Component() = default;
	};

	class ObjectComponent : public Component
	{
	private:
		Reference<Object> Parent;

		virtual bool Attach(const Pointer<Object>&) override;
		virtual void Detach() override;
	public:
		virtual ~ObjectComponent() override = default;
	};

	inline void ObjectComponent::Detach() { Parent.reset(); }

	inline bool ObjectComponent::Attach(const Pointer<Object>& obj) 
	{
		if (Parent.lock())
		{
			return false;
		}
		Parent = obj; 
		return true;
	}

	class ActorComponent : public Component
	{
	private:
		Reference<Actor> Parent;

		virtual bool Attach(const Pointer<Object>&) override;
		virtual void Detach() override;
	public:
		virtual ~ActorComponent() override = default;
	};

	inline void ActorComponent::Detach() { Parent.reset(); }

	inline bool ActorComponent::Attach(const Pointer<Object>& ptr)
	{
		if (!dynamic_cast<Actor*>(ptr.get()))
		{
			return false;
		}

		Parent = DynamicPointerCast<Actor, Object>(ptr);
		return Parent.lock() != nullptr;
	}
};


	// REMEMBER: The compiler really hates it when templated methods are not defined in the header

	template<class C>
	inline bool Engine::Object::HasComponentOfClass()
	{
		type_assert(Engine::Component, C, "Class must be derived from Component");

		// I wondered why adding components didn't work and dound I forgot to implement the method
		// and instead left it as a placeholder with this as only instruction
		//return true;
		// STUPID

		for (Pointer<Component> ptr : Components)
		{
			if (dynamic_cast<C*>(ptr.get()))
			{
				return true;
			}
		}
		return false;
	}

	template<class C>
	inline bool Engine::Object::AddComponentOfClass()
	{
		type_assert(Engine::Component, C, "Class must be derived from Component");

		if (HasComponentOfClass<C>())
		{
			return false;
		}

		Pointer<Component> comp = MakePointer<C>();
		if (!comp->Attach(GetSelf()))
		{
			return false;
		}
		Components.push_back(comp);

		return true;
	}

	template<class C>
	inline void Engine::Object::RemoveComponentOfClass()
	{
		type_assert(Engine::Component, C, "Class must be derived from Component");

		for (size_t i = 0; i < Components.size(); i++)
		{
			if (dynamic_cast<C*>(Components[i].get()))
			{
				Components[i]->Detach();
				Components.erase(i);	// Why not call it remove???
				return;
			}
		}
	}

	template<class C, class... Args>
	inline bool Engine::Object::AddComponentOfClass(Args&&... args)
	{
		type_assert(Engine::Component, C, "Class must be derived from Component");

		if (HasComponentOfClass<C>())
		{
			debug_print("Duplicate component class");
			return false;
		}

		Pointer<Component> comp = MakePointer<C>(args);
		if (!comp->Attach(GetSelf))
		{
			debug_print("Attachment failed");
			return false;
		}
		Components.push_back(comp);

		return true;
	}

	template<class C>
	inline Engine::Reference<C> Engine::Object::GetComponentOfClass()
	{
		type_assert(Engine::Component, C, "Class must be derived from Component");

		Reference<C> ret;
		for (size_t i = 0; i < Components.size(); i++)
		{
			if (dynamic_cast<C*>(Components[i].get()))
			{
				Reference<C> ret = DynamicPointerCast<Engine::ObjectComponent, Engine::Component>(Components[i]);
				return ret;
			}
		}

		ret = MakeReference<C>();	// Nasty disgusting workaround to return null
		return ret;
	}

#endif