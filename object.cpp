#include "CoreEngine.h"

using namespace engine;

bool object::HasTag(const string& tag)
{
	for (const string& t : tags)
	{
		if (tag == t)
		{
			return true;
		}
	}

	return false;
}

bool object::AddTag(const string& tag)
{
	if (HasTag(tag))
	{
		return false;
	}

	tags.push_front(tag);
	return true;
}

object::~object()
{
	for (pointer<component> ptr : components)
	{
		ptr.get()->DetachFromParent();	// Makes sure the component's ptr to parent is set to null
		ptr.reset();
	}
}

/*
bool object::HasComponent(const pointer<component>& comp)
{
	for (pointer<component> c : components)
	{
		if (c == comp)
		{
			return true;
		}
	}

	return false;
}

bool object::AddComponent(const pointer<component>& comp)
{
	if (!comp)
	{
		return false;
	}

	auto& type = typeid(*comp.get());
	for (pointer<component> c : components)
	{
		if (type == typeid(*c.get()))
		{
			return false;
		}
	}

	// TODO: Add the component
	return true;
}
*/