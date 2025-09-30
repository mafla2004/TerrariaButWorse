#include "CoreEngine.h"

using namespace engine;

// --------------------------------------------------
// OBJECT COMPONENT
// --------------------------------------------------

bool ObjectComponent::AttachTo(const pointer<object>& obj)
{
	if (GetParent() != nullptr && !IsAttachedTo(obj))
	{
		return false;
	}

	if (IsAttachedTo(obj))
	{
		return true;
	}

	parent = obj;
	return true;
}

void ObjectComponent::DetachFromParent()
{
	parent.reset();
}

// --------------------------------------------------
// ACTOR COMPONENT
// --------------------------------------------------

bool ActorComponent::AttachTo(const pointer<object>& obj)
{
	if (!dynamic_cast<actor*>(obj.get()))
	{
		return false;
	}

	if (GetParent() != nullptr && !IsAttachedTo(obj))
	{
		return false;
	}

	if (IsAttachedTo(obj))
	{
		return true;
	}

	parent = dynamic_pointer_cast<actor>(obj);
	return true;
}