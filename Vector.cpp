#include "LinearAlgebra.h"

double Vector2::Length() const
{
	return sqrt(x * x + y* y);
}

double Vector3::Length() const
{
	return sqrt(x * x + y * y + z * z);
}