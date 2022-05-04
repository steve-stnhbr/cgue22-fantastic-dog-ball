#include "RenderObject.h"

#include <memory>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btMotionState.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ranges>
#include <glm/gtx/string_cast.hpp>

#include "Utils.h"

RenderObject::RenderObject(Render::Mesh mesh_, Material::Material* material_, const std::string& name_) :
	mesh(mesh_),
	material(material_),
	name(name_),
	transform(glm::mat4(1))
{
	buildVAO();
}

void RenderObject::init()
{
	
}

void RenderObject::update(unsigned long frame, float dTime)
{
	for (auto pair : decorations) {
		pair.second->update(frame, dTime);
	}


	Loggger::fatal("Transform: %s", glm::to_string(transform).c_str());
}


void RenderObject::add(Decoration::Decoration& decoration_)
{
	decoration_.bind(this);
	//decorations.insert({ typeid(decoration_).name(), std::make_unique<Decoration::Decoration>(decoration_) });
	decorations[typeid(decoration_).name()] = &decoration_;
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
	pScale += {s.x, s.y, s.z};
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
	pTranslate += btVector3(v.x, v.y, v.z);
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
	pRotate = Utils::toBT(glm::rotate(glm::mat4(Utils::toGLM(pRotate)), angle, axes));
	return this;
}


void Decoration::Decoration::bind(RenderObject* object_)
{
	object = object_;
	init();
}

void Decoration::Physics::init()
{
	if (object == nullptr) throw new std::logic_error("");

	pTransform = new btDefaultMotionState(btTransform(object->pRotate, object->pTranslate));
	pShape->setLocalScaling(object->pScale);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		pMass,              // mass, in kg. 
		pTransform,
		pShape,				// collision shape of body
		btVector3(0, 0, 0)	// local inertia
	);
	pBody = new btRigidBody(rigidBodyCI);

	pWorld->addRigidBody(pBody);
}

void Decoration::Physics::update(unsigned frame, float dTime)
{
	btTransform transform;
	pBody->getMotionState()->getWorldTransform(transform);
	glm::mat4 mat;
	transform.getOpenGLMatrix(glm::value_ptr(mat));
	object->transform = mat;
}

Decoration::Physics::Physics(btDynamicsWorld* pWorld_, btCollisionShape* pShape_, float pMass_) : pShape(pShape_), pMass(pMass_), pWorld(pWorld_)
{
}


