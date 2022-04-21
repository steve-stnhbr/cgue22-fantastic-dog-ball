#include "LightSource_alt.h"

template <typename V>
Light_::Light<V>::Light(V values_): values(values_)
{
	buffer.create(sizeof(V));
}

template <typename V>
const V* Light_::Light<V>::getValues() const
{
	return values;
}

template <typename V>
void Light_::Light<V>::update(V* values_)
{
	buffer.update(&values_);
}

template <typename V>
UncheckedUniformBuffer Light_::Light<V>::getBuffer() const
{
	return buffer;
}

template <typename V>
Light_::Lights<V>::Lights(std::vector<Light<V>> elements_) : elements(elements_)
{
	for (Light<V> element : elements)
	{
		values.push_back(element.getValues());
	}
}

template <typename V>
void Light_::Lights<V>::add(Light<V> e)
{
	elements.push_back(e);
	values.push_back(e.getValues());
}






Light_::Point::Point() : Point(PValues{})
{
}


Light_::Point::Point(PValues values_): Light<PValues>(values_)
{
}

Light_::Point::~Point() = default;


Light_::Directional::Directional(): Directional(DValues{})
{
}


Light_::Directional::Directional(DValues values_) : Light<DValues>(values_)
{
}

Light_::Directional::~Directional() = default;


Light_::Spot::Spot(): Spot(SValues{})
{
}

Light_::Spot::Spot(SValues values_): Light<SValues>(values_)
{
}

Light_::Spot::~Spot() = default;




