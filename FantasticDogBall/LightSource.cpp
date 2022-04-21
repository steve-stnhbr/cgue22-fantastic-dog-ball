#include "LightSource.h"

template <typename V>
Light::Light<V>::Light(V values_): values(values_)
{
	buffer.create(sizeof(V));
}


