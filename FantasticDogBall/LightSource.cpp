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
	SHADOW_PROGRAM_2D = Shaders::Program({ "./shadow.vert", "./shadow.frag" });
	data = {
		glm::vec4(direction_.x, direction_.y, direction_.z, 0),
		glm::vec4(ambient_.x, ambient_.y, ambient_.z, 0),
		glm::vec4(diffuse_.x, diffuse_.y, diffuse_.z, 0),
		glm::vec4(specular_.x, specular_.y, specular_.z, 0)
	};
}

Texture::Texture* Light::Directional::generateShadowMap(const std::vector<RenderObject>& objects)
{
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// 1. first render to depth map
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	SHADOW_PROGRAM_2D.use();

	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); 
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	SHADOW_PROGRAM_2D.setMatrix4("lightSpace", lightSpaceMatrix);
	for (RenderObject element : objects)
	{
		element.update();

		Loggger::info("\t%s", element.name.c_str());

		// bind program
		auto prog = element.material->getProgram();
		prog.use();
		element.material->bind(prog);
		Utils::checkError();
		glBindVertexArray(element.vaoID);
		glBindVertexBuffer(0, element.vboID, 0, sizeof(Vertex));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element.eboID);
		Utils::checkError();
		// draw
		glDrawElements(GL_TRIANGLES, element.mesh.index_array.size(), GL_UNSIGNED_INT, nullptr);
		Utils::checkError();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shadowMap->glID = depthMap;
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










