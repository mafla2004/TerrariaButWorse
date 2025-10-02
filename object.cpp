#include "CoreEngine.h"

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