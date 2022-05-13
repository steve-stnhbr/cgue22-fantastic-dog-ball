#include "LightSource.h"

#include "Material.h"
#include "Render.h"

unsigned Light::Light::SHADOW_MAP_RESOLUTION = 1024;
unsigned Light::Light::SHADOW_FRAMEBUFFER = 0;
Shaders::Program Light::Light::SHADOW_PROGRAM = std::vector<std::string>{ "./shadow.vert", "./shadow.frag" };


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

glm::mat4 Light::Directional::getLightSpace()
{
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::inverse<float>(direction), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
	Globals::NUM_POINT_LIGHTS++;
}

void Light::Lights::add(const Directional d)
{
	dLights.push_back(d);
	Globals::NUM_DIRECTIONAL_LIGHTS++;
}
void Light::Lights::add(const Spot s)
{
	sLights.push_back(s);
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
		spot.position = { -1000, -1000, -1000, 1 };
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

Light::Light::Light() : Light(true)
{
}

Light::Light::Light(bool useShadowMap)
{
	shadowMap = 0;
	if (useShadowMap) {
		if (SHADOW_FRAMEBUFFER == 0) {
			glCreateFramebuffers(1, &SHADOW_FRAMEBUFFER);
			glNamedFramebufferTexture(SHADOW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap.glID);
		}
		
		shadowMap = Texture::Texture{ SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, GL_FLOAT, 1 };
	}
}

void Light::Light::generateShadowMap()
{
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
	glBindFramebuffer(GL_FRAMEBUFFER, SHADOW_FRAMEBUFFER);
	glClear(GL_DEPTH_BUFFER_BIT);
	SHADOW_PROGRAM.use();
	SHADOW_PROGRAM.setMatrix4("lightSpace", getLightSpace());
	glNamedFramebufferDrawBuffer(SHADOW_FRAMEBUFFER, GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
