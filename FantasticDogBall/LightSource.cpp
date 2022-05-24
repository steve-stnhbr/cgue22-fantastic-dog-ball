#include "LightSource.h"

#include "Material.h"
#include "Render.h"
#include "RenderObject.h"

#include <glm/gtc/matrix_transform.hpp>

Shaders::Program createShadowProgram();

unsigned Light::Light::SHADOW_MAP_RESOLUTION = 1024;
unsigned Light::Light::SHADOW_FRAMEBUFFER = 0;
Shaders::Program Light::Light::SHADOW_PROGRAM;

Light::Point::Point(glm::vec3 position_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, bool castShadow): Light::Light(castShadow)
{
	data = {
		glm::vec4(position_.x, position_.y, position_.z, 0),
		glm::vec4(constant_, linear_, quadratic_, 0),
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0)
	};
}

glm::mat4 Light::Point::getLightSpace() const {
	return glm::mat4(1);
}

Light::Directional::Directional(glm::vec3 direction_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, bool castShadow) : Light::Light(castShadow)
{
	data = {
		glm::vec4(direction_.x, direction_.y, direction_.z, 0),
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0)
	};
}

glm::mat4 Light::Directional::getLightSpace() const
{
	float near_plane = .1f, far_plane = 7.5f;
	glm::mat4 depthProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(data.direction), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	return depthProjectionMatrix * depthViewMatrix;
}

Light::Spot::Spot(glm::vec3 position_, glm::vec3 direction_, float cutOff_, float outerCutOff_, float constant_, float linear_, float quadratic_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, bool castShadow) : Light::Light(castShadow)
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

glm::mat4 Light::Spot::getLightSpace() const {
	return glm::mat4(1);
}

Light::Lights::Lights()
{
	finalized = false;
}

void Light::Lights::add(const Point p)
{
	pLights.push_back(p);
	pData.push_back(p.data);
	allLights.push_back(&p);
	Globals::NUM_POINT_LIGHTS++;
}

void Light::Lights::add(const Directional d)
{
	dLights.push_back(d);
	dData.push_back(d.data);
	allLights.push_back(&d);
	Globals::NUM_DIRECTIONAL_LIGHTS++;
}
void Light::Lights::add(const Spot s)
{
	sLights.push_back(s);
	sData.push_back(s.data);
	allLights.push_back(&s);
	Globals::NUM_SPOT_LIGHTS++;
}


void Light::Lights::bind(Shaders::Program& prog)
{
	if(!finalized)
	{
		finalize();
	}

	/*
	prog.setUniform("PointLights", pBuffer);
	prog.setUniform("DirectionalLights", dBuffer);
	prog.setUniform("SpotLights", sBuffer);
	*/


	for (auto i = 0; i < pLights.size(); i++) {
		prog.setVector4(Utils::string_format("pLights[%i].position", i), pLights[i].data.position);
		prog.setVector4(Utils::string_format("pLights[%i].attenuation", i), pLights[i].data.attenuation);
		prog.setVector4(Utils::string_format("pLights[%i].ambient", i), pLights[i].data.ambient);
		prog.setVector4(Utils::string_format("pLights[%i].diffuse", i), pLights[i].data.diffuse);
		prog.setVector4(Utils::string_format("pLights[%i].specular", i), pLights[i].data.specular);

		prog.setBool(Utils::string_format("pLights[%i].castsShadow", i), pLights[i].castShadow);
		prog.setMatrix4(Utils::string_format("pLights[%i].lightSpace", i), pLights[i].getLightSpace());

		if(pLights[i].castShadow)
			prog.setTexture(Utils::string_format("pLights[%i].shadowMap", i), pLights[i].shadowMap);
	}

	for (auto i = 0; i < dLights.size(); i++) {
		prog.setVector4(Utils::string_format("dLights[%i].direction", i), dLights[i].data.direction);
		prog.setVector4(Utils::string_format("dLights[%i].ambient", i), dLights[i].data.ambient);
		prog.setVector4(Utils::string_format("dLights[%i].diffuse", i), dLights[i].data.diffuse);
		prog.setVector4(Utils::string_format("dLights[%i].specular", i), dLights[i].data.specular);

		prog.setBool(Utils::string_format("dLights[%i].castsShadow", i), dLights[i].castShadow);
		prog.setMatrix4(Utils::string_format("dLights[%i].lightSpace", i), dLights[i].getLightSpace());

		if (dLights[i].castShadow)
			prog.setTexture(Utils::string_format("dLights[%i].shadowMap", i), dLights[i].shadowMap);
	}
	for (auto i = 0; i < sLights.size(); i++) {
		prog.setVector4(Utils::string_format("sLights[%i].position", i), sLights[i].data.position);
		prog.setVector4(Utils::string_format("sLights[%i].direction", i), sLights[i].data.direction);
		prog.setVector4(Utils::string_format("sLights[%i].cutoff", i), sLights[i].data.cutoff);
		prog.setVector4(Utils::string_format("sLights[%i].attenuation", i), sLights[i].data.attenuation);
		prog.setVector4(Utils::string_format("sLights[%i].ambient", i), sLights[i].data.ambient);
		prog.setVector4(Utils::string_format("sLights[%i].diffuse", i), sLights[i].data.diffuse);
		prog.setVector4(Utils::string_format("sLights[%i].specular", i), sLights[i].data.specular);

		prog.setBool(Utils::string_format("sLights[%i].castsShadow", i), sLights[i].castShadow);
		prog.setMatrix4(Utils::string_format("sLights[%i].lightSpace", i), sLights[i].getLightSpace());

		if (sLights[i].castShadow)
			prog.setTexture(Utils::string_format("sLights[%i].shadowMap", i), sLights[i].shadowMap);
	}


}

std::vector<const Light::Light*> Light::Lights::all()
{
	return allLights;
}

void Light::Lights::finalize()
{
	if (pLights.empty()) {
		Point p;
		p.data.diffuse = { .1, .1, .1, 0 };
		add(p);
	}
	if (dLights.empty()) {
		Directional d;
		d.data.diffuse = { .1, .1, .1, 0 };
		add(d);
	}
	if (sLights.empty()) {
		Spot spot;
		spot.data.diffuse = {.1, .1, .1, 0};
		spot.data.position = { -1000, -1000, -1000, 1 };
		add(spot);
	}

	Light::Light::initProgram();
	

	Globals::NUM_POINT_LIGHTS = pLights.size();
	Globals::NUM_DIRECTIONAL_LIGHTS = dLights.size();
	Globals::NUM_SPOT_LIGHTS = sLights.size();

	pBuffer.create(Globals::NUM_POINT_LIGHTS * sizeof(Point::Data));
	pBuffer.update(pData.data());
	dBuffer.create(Globals::NUM_DIRECTIONAL_LIGHTS * sizeof(Directional::Data));
	dBuffer.update(dData.data());
	sBuffer.create(Globals::NUM_SPOT_LIGHTS * sizeof(Spot::Data));
	sBuffer.update(sData.data());

	Material::initPrograms();

	finalized = true;
}

Light::Light::Light() : Light(false)
{
}

Light::Light::Light(bool useShadowMap): castShadow(useShadowMap)
{
	if (useShadowMap) {
		shadowMap = Texture::Texture{ SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, 1, false };
	}
}

void Light::Light::initProgram() {
	if (SHADOW_FRAMEBUFFER == 0) {
		glCreateFramebuffers(1, &SHADOW_FRAMEBUFFER);
		glNamedFramebufferDrawBuffer(SHADOW_FRAMEBUFFER, GL_NONE);
		Utils::checkError();
		glNamedFramebufferReadBuffer(SHADOW_FRAMEBUFFER, GL_NONE);
		Utils::checkError();
	}

	if (SHADOW_PROGRAM.ID == 0) {
		try {
			SHADOW_PROGRAM = { std::vector<std::string>{ "./shadow.vert", "./shadow.frag" } };
		}
		catch (std::exception e) {
			Loggger::error(e.what());
		}
	}
}

Texture::Texture Light::Light::generateShadowMap(const std::vector<RenderObject>& obj) const
{
	if (!castShadow) return false;
	glEnable(GL_DEPTH_TEST);
	glNamedFramebufferTexture(SHADOW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap.glID, 0);
	Utils::checkError();
	GLenum status = glCheckNamedFramebufferStatus(SHADOW_FRAMEBUFFER, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Loggger::error("FB error, status: 0x%x\n", status);
	}
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
	SHADOW_PROGRAM.use();
	SHADOW_PROGRAM.setMatrix4("lightSpace", getLightSpace());
	glBindFramebuffer(GL_FRAMEBUFFER, SHADOW_FRAMEBUFFER);
	glClear(GL_DEPTH_BUFFER_BIT);
	Utils::checkError();

	for (auto element : obj) {
		SHADOW_PROGRAM.setMatrix4("model", element.transform);
		glBindVertexArray(element.vaoID);
		glBindVertexBuffer(0, element.vboID, 0, sizeof(Vertex));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element.eboID);
		glDrawElements(GL_TRIANGLES, element.mesh.index_array.size(), GL_UNSIGNED_INT, nullptr);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Utils::checkError();

	return shadowMap;
}
