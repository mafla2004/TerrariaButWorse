#include "CoreEngine.h"
#include <iostream>

int main()
{
	using namespace Engine;

	// Check correct creation of pointer to object
	Pointer<Object> ptr = MakePointer<Object>();

	std::cout << "Object address: " << ptr << std::endl;
}