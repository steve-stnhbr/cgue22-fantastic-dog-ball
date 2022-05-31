
#include "Material.h"


Shaders::Program Material::StaticMaterial::program;
Shaders::Program Material::TextureMaterial::program;

Material::StaticMaterial::StaticMaterial()
{
	createBuffer();
}

Material::StaticMaterial::StaticMaterial(const glm::vec4 color_, const float diffuse_, const float specular_, const float shininess_) :
	vals({ color_, diffuse_, specular_, shininess_ })
{
	createBuffer();
	Utils::checkError();
}

Shaders::Program Material::StaticMaterial::getProgram()
{
	return program;
}

void Material::StaticMaterial::createBuffer()
{
	buffer.create(sizeof(Values));
}

void Material::StaticMaterial::initProgram()
{
	program = Shaders::Program( { vertexShader, colorFragmentShader } );
}


void Material::StaticMaterial::bind(Shaders::Program& p)
{
	buffer.update(sizeof(Values), &vals);
	Utils::checkError();
	p.setUniform("Material", buffer);
	Utils::checkError();
}


/**
 * This function is responsible for assigning streams of vertex-attributes
 * location = 0, binding = 0: postition
 * location = 1, binding = 0: normal
 * location = 2, binding = 0: color
 */
void Material::StaticMaterial::assignVertexAttributes(unsigned vao)
{
	// --- postition ---
	glEnableVertexArrayAttrib(vao, 0);
	Utils::checkError();
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 0, 0);
	Utils::checkError();
	// --- normal ---
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 1, 0);
	Utils::checkError();
	glEnableVertexArrayAttrib(vao, 1);
	Utils::checkError();
	// --- color ---
	glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coordinate));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 2, 0);
	Utils::checkError();
	glEnableVertexArrayAttrib(vao, 2);
	Utils::checkError();
}


Shaders::Program Material::TextureMaterial::getProgram()
{
	return program;
}

void Material::TextureMaterial::bind(Shaders::Program& p)
{

	p.setTexture("color", color);
	p.setTexture("normal", normal);
	p.setTexture("diffuse", diffuse);
	p.setTexture("specular", specular);
	p.setFloat("shininess", shininess);
}

/**
 * This function is responsible for assigning streams of vertex-attributes
 * location = 0, binding = 0: postition
 * location = 1, binding = 0: normal
 * location = 2, binding = 0: texture_coordinate
 */
void Material::TextureMaterial::assignVertexAttributes(unsigned vao)
{
	// --- postition ---
	glEnableVertexArrayAttrib(vao, 0);
	Utils::checkError();
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 0, 0);
	Utils::checkError();
	// --- normal ---
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 1, 0);
	Utils::checkError();
	glEnableVertexArrayAttrib(vao, 1);
	Utils::checkError();
	// --- color ---
	glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coordinate));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 2, 0);
	Utils::checkError();
	glEnableVertexArrayAttrib(vao, 2);
	Utils::checkError();
}

void Material::TextureMaterial::initProgram()
{
	TextureMaterial::program = Shaders::Program({ vertexShader, textureFragmentShader });
}

Shaders::Program Material::ProceduralMaterial::getProgram()
{
	return program;
}

/**
 * This function is responsible for assigning streams of vertex-attributes
 * location = 0, binding = 0: postition
 * location = 1, binding = 0: normal
 */
void Material::ProceduralMaterial::assignVertexAttributes(unsigned vao)
{
	// --- postition ---
	glEnableVertexArrayAttrib(vao, 0);
	Utils::checkError();
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 0, 0);
	Utils::checkError();
	// --- normal ---
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	Utils::checkError();
	glVertexArrayAttribBinding(vao, 1, 0);
	Utils::checkError();
	glEnableVertexArrayAttrib(vao, 1);
	Utils::checkError();
}