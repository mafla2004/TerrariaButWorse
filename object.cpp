#include "CoreEngine.h"

template<class C>
bool Engine::Object::HasComponentOfClass()
{
	type_assert(Engine::Component, C, "Class must be derived from Component");

	return true;
}

template<class C>
bool Engine::Object::AddComponentOfClass()
{
	type_assert(Engine::Component, C, "Class must be derived from Component");

	if (HasComponentOfClass<C>())
	{
		return false;
	}
	
	return true;
}

template<class C>
void Engine::Object::RemoveComponentOfClass()
{
	type_assert(Engine::Component, C, "Class must be derived from Component");

	for (size_t i = 0; i < Components.size(); i++)
	{
		if (dynamic_cast<C*>(Components[i].get()))
		{
			Components.erase(i);	// Why not call it remove???
			return;
		}
	}
}

void Engine::Object::tick(float DeltaTime)
{
	Component* component;
	for (size_t i = 0; i < Components.size(); i++)
	{
		component = Components[i].get();

		component->tick(DeltaTime);
	}
	component = nullptr;
}