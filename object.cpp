#include "CoreEngine.h"

template<class C>
bool Engine::Object::HasComponentOfClass()
{
	// TODO: Add logic
	return true;
}

template<class C>
bool Engine::Object::AddComponentOfClass()
{
	if (HasComponentOfClass<C>())
	{
		return false;
	}

	// TODO: Add logic
	return true;
}
