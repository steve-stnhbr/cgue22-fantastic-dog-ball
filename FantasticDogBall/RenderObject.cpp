#include "RenderObject.h"

#include <memory>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btMotionState.h>

#include "Utils.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

RenderObject::RenderObject(Render::Mesh mesh_, Material::Material* material_, const std::string& name_) :
	mesh(mesh_),
	material(material_),
	name(name_),
	transform(glm::mat4(1))
{
	buildVAO();
}

void RenderObject::update()
{
}


void RenderObject::add(Decoration::Decoration& decoration_)
{
	decoration_.bind(this);
	// decorations[typeid(decoration_)] = decoration_;
}

void RenderObject::buildVAO() const
{
	glCreateBuffers(1, (GLuint*) &vboID);
	glNamedBufferStorage(vboID, mesh.vertex_array.size() * sizeof(Vertex), mesh.vertex_array.data(), GL_DYNAMIC_STORAGE_BIT);
	Utils::checkError();

	glCreateBuffers(1, (GLuint*)&eboID);
	glNamedBufferStorage(eboID, mesh.index_array.size() * sizeof(unsigned), mesh.index_array.data(), GL_DYNAMIC_STORAGE_BIT);
	Utils::checkError();

	using vertexType = std::decay_t<decltype(*mesh.vertex_array.begin())>;
	using indexType = std::decay_t<decltype(*mesh.index_array.begin())>;

	glCreateVertexArrays(1, (GLuint*)&vaoID);
	glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(vertexType));
	glVertexArrayElementBuffer(vaoID, eboID);
	
	material->assignVertexAttributes(vaoID);
}

RenderObject* RenderObject::scale(float x, float y, float z)
{
	scale({ x,y,z });
	return this;
}

RenderObject* RenderObject::scale(glm::vec3 s)
{
	transform = glm::scale(transform, s);
	return this;
}

void RenderObject::doTransform()
{

}

RenderObject* RenderObject::translate(float x, float y, float z)
{
	translate({ x, y, z });
	return this;
}

RenderObject* RenderObject::translate(glm::vec3 v)
{
	transform = glm::translate(transform, v);
	return this;
}

RenderObject* RenderObject::rotate(float x, float y, float z)
{
	rotate(x, { 1, 0, 0 });
	rotate(y, { 0, 1, 0 });
	rotate(z, { 0, 0, 1 });
	return this;
}

RenderObject* RenderObject::rotate(float angle, glm::vec3 axes)
{
	transform = glm::rotate(transform, angle, axes);
	return this;
}


void Decoration::Decoration::bind(RenderObject* object_)
{
	object = object_; 
}

void Decoration::Physics::update(unsigned frame, float dTime)
{
}

Decoration::Compute::ComputeType::ComputeType(Shaders::Program program_) : program(program_)
{
}

Decoration::Compute::CatmullClarkSubdivision::CatmullClarkSubdivision() : CatmullClarkSubdivision(1)
{
}

Decoration::Compute::CatmullClarkSubdivision::CatmullClarkSubdivision(unsigned short levels) : ComputeType({ {"./cc_sub.geo"} })
{
	isGeometry = true;
	if (levels > 3)
		Loggger::warn("Catmull-Clark subdivision was called with %us levels. The computation may take a while", levels);
}

Decoration::Compute::SimpleSubdivision::SimpleSubdivision() : SimpleSubdivision(1)
{
}

Decoration::Compute::SimpleSubdivision::SimpleSubdivision(unsigned short levels) : ComputeType({ {"./si_sub.geo"}})
{
	isGeometry = true;
	if (levels > 5)
		Loggger::warn("Simple subdivision was called with %us levels. The computation may take a while", levels);
}

Decoration::Compute::Compute(ComputeType type) : Compute({type})
{
}

Decoration::Compute::Compute(std::vector<ComputeType> types_) : types(types_)
{
}

void Decoration::Compute::bind(RenderObject* obj) {
	for (ComputeType type : types) {
		if (type.isGeometry) {
			obj->mesh = computeGeometry(type.program, obj);
		}
	}
}

Render::Mesh computeGeometry(Shaders::Program program, RenderObject* obj) {
	unsigned int fb;
	glCreateTransformFeedbacks(1, &fb);
	UncheckedUniformBuffer fbb;
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbb.id);
	glEnable(GL_RASTERIZER_DISCARD);
	obj->material->assignVertexAttributes(fbb.id);
	program.use();

	glBeginTransformFeedback(GL_QUADS);

}

