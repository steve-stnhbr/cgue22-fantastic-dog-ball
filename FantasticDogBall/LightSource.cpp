#include "LightSource.h"

#include "Material.h"
#include "Render.h"

Light::Point::Point(glm::vec3 position_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_)
{
	data = {
		glm::vec4(position_.x, position_.y, position_.z, 0),
		glm::vec4(constant_, linear_, quadratic_, 0),
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0)
	};
}

Texture::Cubemap* Light::Point::generateShadowMap(const std::vector<RenderObject>& objects)
{
	return shadowMap;
}

Light::Directional::Directional(glm::vec3 direction_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_)
{
	data = {
		glm::vec4(direction_.x, direction_.y, direction_.z, 0),
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0)
	};
}

Texture::Texture* Light::Directional::generateShadowMap(const std::vector<RenderObject>& objects)
{
	return shadowMap;
}

Light::Spot::Spot(glm::vec3 position_, glm::vec3 direction_, float cutOff_, float outerCutOff_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_)
{
	data = {
		glm::vec4(position_.x, position_.y, position_.z, 0),
		glm::vec4(direction_.x, direction_.y, direction_.z, 0),
		glm::vec4(cutOff_, outerCutOff_, 0, 0),
		glm::vec4(constant_, linear_, quadratic_, 0),
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0)
	};
}
Texture::Texture* Light::Spot::generateShadowMap(const std::vector<RenderObject>& objects)
{



	return shadowMap;
}

Light::Lights::Lights()
{
	finalized = false;
}

void Light::Lights::add(const Point p)
{
	pLights.push_back(p.data);
	Globals::NUM_POINT_LIGHTS++;
}

void Light::Lights::add(const Directional d)
{
	dLights.push_back(d.data);
	Globals::NUM_DIRECTIONAL_LIGHTS++;
}
void Light::Lights::add(const Spot s)
{
	sLights.push_back(s.data);
	Globals::NUM_SPOT_LIGHTS++;
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
	if (sLights.empty()) {
		Spot spot;
		spot.data.position = { -1000, -1000, -1000, 1 };
		add(spot);
	}

	pBuffer.create(Globals::NUM_POINT_LIGHTS * sizeof(Point));
	pBuffer.update(pLights.data());
	dBuffer.create(Globals::NUM_DIRECTIONAL_LIGHTS * sizeof(Directional));
	dBuffer.update(dLights.data());
	sBuffer.create(Globals::NUM_SPOT_LIGHTS * sizeof(Spot));
	sBuffer.update(sLights.data());

	Material::initPrograms();

	finalized = true;
}










