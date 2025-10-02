#include "CoreEngine.h"

int main()
{
	using namespace Engine;

	// Check correct creation of pointer to object
	Pointer<Object> ptr = MakePointer<Object>();

	std::cout << "Object address: " << ptr << std::endl;

	// Check addition of components
	
	// Adding an object component - should result true
	if (!ptr->AddComponentOfClass<ObjectComponent>())
	{
		std::cout << "ERROR! Component couldn't be added!" << std::endl;
		return 0;
	}
	std::cout << "Component was added successfully at address: " << 
		ptr->GetComponentOfClass<ObjectComponent>().lock() << std::endl;

	// Adding an actor component - should result false
	if (ptr->AddComponentOfClass<ActorComponent>())
	{
		std::cout << "ERROR! Shouldn't have been able to add it" << std::endl;
	}
}