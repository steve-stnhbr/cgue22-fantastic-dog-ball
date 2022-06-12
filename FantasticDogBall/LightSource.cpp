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
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0) * (castShadow ? 2.5f : 1),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0) * (castShadow ? 2.5f : 1),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0) * (castShadow ? 2.5f : 1)
	};
}

Light::Point::Data::Data(glm::vec4 position, glm::vec4 attenuation, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular):
	position(position),
	attenuation(attenuation),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
}

glm::mat4 Light::Point::getLightSpace() const {
	return glm::mat4(1);
}

void Light::Point::generateShadowMap(const RenderObject::renderList& objs) {
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, SHADOW_FRAMEBUFFER);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	float aspect = (float)SHADOW_MAP_RESOLUTION / (float)SHADOW_MAP_RESOLUTION;
	float near = 1.0f;
	float far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
	std::vector<glm::mat4> shadowTransforms;
	glm::vec3 lightPos(data.position);
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

}

Light::Directional::Directional(glm::vec3 direction_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, bool castShadow) : Light::Light(castShadow)
{
	data = Data(
		glm::vec4(direction_.x, direction_.y, direction_.z, 0),
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0)
	);
}

Light::Directional::Data::Data(glm::vec4 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular) : 
	direction(direction),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
}

glm::mat4 Light::Directional::getLightSpace() const
{
	float near_plane = -7.5f, far_plane = 17.5f;
	glm::mat4 depthProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(-data.direction),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	return depthProjectionMatrix * depthViewMatrix;
}

void Light::Directional::generateShadowMap(const RenderObject::renderList& objs) {
	generateShadowMap2D(objs);
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

Light::Spot::Data::Data(glm::vec4 position, glm::vec4 direction, glm::vec4 cutoff, glm::vec4 attenuation, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular) : 
	position(position),
	cutoff(cutoff),
	attenuation(attenuation),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
}

glm::mat4 Light::Spot::getLightSpace() const {
	return glm::mat4(1);
}

void Light::Spot::generateShadowMap(const RenderObject::renderList& objs) {
	generateShadowMap2D(objs);
}

Light::Lights::Lights()
{
	finalized = false;
}

void Light::Lights::add(const Point p)
{
	pLights.push_back(p);
	pData.push_back(p.data);
	Globals::NUM_POINT_LIGHTS++;
}

void Light::Lights::add(const Directional d)
{
	dLights.push_back(d);
	dData.push_back(d.data);
	Globals::NUM_DIRECTIONAL_LIGHTS++;
}
void Light::Lights::add(const Spot s)
{
	sLights.push_back(s);
	sData.push_back(s.data);
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

void Light::Lights::generateAllShadowMaps(const RenderObject::renderList& objs)
{
	for (auto& l : dLights)
		l.generateShadowMap(objs);
	/*
	for (auto& l : sLights)
		l.generateShadowMap(objs);
	for (auto& l : pLights)
		l.generateShadowMap(objs);
		*/
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

Light::Light::Light(bool useShadowMap): castShadow(useShadowMap), shadowMapID(0)
{
	if (useShadowMap) {
		glGenTextures(1, &shadowMapID);
		glBindTexture(GL_TEXTURE_2D, shadowMapID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

Texture::Texture* Light::Light::generateShadowMap2D(const RenderObject::renderList& objects)
{ 
	if (!castShadow) return new Texture::Texture(1);
	glBindFramebuffer(GL_FRAMEBUFFER, SHADOW_FRAMEBUFFER);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// 1. first render to depth map
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
	glBindFramebuffer(GL_FRAMEBUFFER, SHADOW_FRAMEBUFFER);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_RASTERIZER_DISCARD);
	SHADOW_PROGRAM.use();
	SHADOW_PROGRAM.setMatrix4("lightSpace", getLightSpace());
	for (RenderObject* element : objects) {
		SHADOW_PROGRAM.setMatrix4("model", element->transform);
		element->draw(SHADOW_PROGRAM);
		/*
		Utils::checkError();
		glBindVertexArray(element->vaoID);
		glBindVertexBuffer(0, element->mesh->vboID, 0, sizeof(Vertex));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->mesh->eboID);
		Utils::checkError();
		// draw
		glDrawElements(GL_TRIANGLES, element->mesh->index_array.size(), GL_UNSIGNED_INT, nullptr);
		Utils::checkError();
		*/
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_RASTERIZER_DISCARD);
	Texture::Texture* tex = new Texture::Texture();
	tex->defined = true;
	tex->glID = shadowMapID;
	shadowMap = tex;
	return shadowMap;
}
