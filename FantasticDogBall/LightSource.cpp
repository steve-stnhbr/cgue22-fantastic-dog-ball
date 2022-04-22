#include "LightSource.h"

#include "Utils.h"

Light::Point::Point(glm::vec3 position_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_) :
	position(position_), constant(constant_), linear(linear_), quadratic(quadratic_), ambient(ambient_), diffuse(diffuse_), specular(specular_)
{
	
}


Light::Directional::Directional(glm::vec3 direction_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_) :
	direction(direction_), ambient(ambient_), diffuse(diffuse_), specular(specular_)
{
	
}

Light::Spot::Spot(glm::vec3 position_, glm::vec3 direction_, float cutOff_, float outerCutOff_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_) :
	position(position_), direction(direction_), cutOff(cutOff_), outerCutOff(outerCutOff_), constant(constant_), linear(linear_), quadratic(quadratic_), ambient(ambient_), diffuse(diffuse_), specular(specular_)
{
	
}



Light::Lights::Lights()
{
	buffer.create(NUM_LIGHTS * sizeof(Point) + NUM_LIGHTS * sizeof(Directional) + NUM_LIGHTS * sizeof(Spot));
}

void Light::Lights::add(Point p)
{
	pLights.push_back(p);
	update();
}

void Light::Lights::add(Directional d)
{
	dLights.push_back(d);
	update();
}
void Light::Lights::add(Spot s)
{
	sLights.push_back(s);
	update();
}

void Light::Lights::update()
{
	pLights.resize(NUM_LIGHTS);
	dLights.resize(NUM_LIGHTS);
	sLights.resize(NUM_LIGHTS);

	data.pLights = &pLights[0];
	data.dLights = &dLights[0];
	data.sLights = &sLights[0];

	buffer.update(&data);
}










