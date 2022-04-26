#include "LightSource.h"

#include "Material.h"
#include "Render.h"

unsigned Light::Light::NUM_SPOT_LIGHTS = 0;
unsigned Light::Light::NUM_DIRECTIONAL_LIGHTS = 0;
unsigned Light::Light::NUM_POINT_LIGHTS = 0;

Light::Point::Point(glm::vec3 position_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_) :
	position(position_.x, position_.y, position_.z, 0),
	attenuation(constant_, linear_, quadratic_, 0),
	ambient(ambient_.x, ambient_.y, ambient_.z, 0),
	diffuse(diffuse_.x, diffuse_.y, diffuse_.z, 0),
	specular(specular_.x, specular_.y, specular_.z, 0)
{
}


Light::Directional::Directional(glm::vec3 direction_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_) :
	direction(direction_.x, direction_.y, direction_.z, 0),
	ambient(ambient_.x, ambient_.y, ambient_.z, 0),
	diffuse(diffuse_.x, diffuse_.y, diffuse_.z, 0),
	specular(specular_.x, specular_.y, specular_.z, 0)
{
	
}

Light::Spot::Spot(glm::vec3 position_, glm::vec3 direction_, float cutOff_, float outerCutOff_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_) :
	position(position_.x, position_.y, position_.z, 0),
	direction(direction_.x, direction_.y, direction_.z, 0),
	cutoff(cutOff_, outerCutOff_, 0, 0),
	attenuation(constant_, linear_, quadratic_, 0),
	ambient(ambient_.x, ambient_.y, ambient_.z, 0),
	diffuse(diffuse_.x, diffuse_.y, diffuse_.z, 0),
	specular(specular_.x, specular_.y, specular_.z, 0)
{
	
}

Light::Lights::Lights()
{
	finalized = false;
}

void Light::Lights::add(const Point p)
{
	pLights.push_back(p);
	Light::NUM_POINT_LIGHTS++;
}

void Light::Lights::add(const Directional d)
{
	dLights.push_back(d);
	Light::NUM_DIRECTIONAL_LIGHTS++;
}
void Light::Lights::add(const Spot s)
{
	sLights.push_back(s);
	Light::NUM_SPOT_LIGHTS++;
}


void Light::Lights::bind(Shaders::Program& prog)
{
	if(!finalized)
	{
		finalize();
	}

	prog.setUniform("PointLights", pBuffer);
	prog.setUniform("DirectionalLights", dBuffer);
	prog.setUniform("SpotLights", sBuffer);
}

void Light::Lights::finalize()
{
	if (pLights.empty())
		add(Point{});
	if (dLights.empty())
		add(Directional{});
	if (sLights.empty())
		add(Spot{});

	pBuffer.create(Light::NUM_POINT_LIGHTS * sizeof(Point));
	pBuffer.update(pLights.data());
	dBuffer.create(Light::NUM_DIRECTIONAL_LIGHTS * sizeof(Directional));
	dBuffer.update(dLights.data());
	sBuffer.create(Light::NUM_SPOT_LIGHTS * sizeof(Spot));
	sBuffer.update(sLights.data());


	Material::initPrograms();

	finalized = true;
}










