#include "LightSource.h"

#include "Utils.h"


template <typename V>
Light::Lights<V>::Lights(std::vector<V> values_) : std::vector<V>(values_)
{
	if (!Utils::instanceof<Light, V>())
		throw std::exception("template to lights can only be type of Light::Light");
}


