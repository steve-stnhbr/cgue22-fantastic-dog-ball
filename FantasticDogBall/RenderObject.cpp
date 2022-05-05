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
	transform(glm::mat4(1)),
	pScale({1, 1, 1})
{
	buildVAO();
}

void RenderObject::init()
{
	for (auto val : decorations.vals) {
		val->init(this);
	}
}

void RenderObject::update(unsigned long frame, float dTime)
{
	for (auto val : decorations.vals) {
		val->update(this, frame, dTime);
	}
}


void RenderObject::add(Decoration::Decoration& decoration_)
{
	decoration_.bind(this);
	decorations.insert(typeid(decoration_).hash_code(), &decoration_);
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

RenderObject* RenderObject::scale(float s)
{
	scale(s, s, s);
	return this;
}


void Decoration::Decoration::bind(RenderObject* object_)
{
}

void Decoration::Physics::init(RenderObject* object)
{
	if (object == nullptr) {
		Loggger::fatal("Object was somehow not yet initialized in init of Decoration");
		exit(-15);
	}

	if (pShape == nullptr) {
		Loggger::trace("Using Convex hull of mesh for %s", object->name.c_str());
		pShape = new btConvexHullShape();
		for (int i = 0; i < object->mesh.vertex_array.size(); i++)
		{
			Vertex v = object->mesh.vertex_array[i];
			btVector3 btv = btVector3(v.position.x, v.position.y, v.position.z);
			((btConvexHullShape*)pShape)->addPoint(btv);
		}
	}

	btTransform t;
	t.setFromOpenGLMatrix(glm::value_ptr(object->transform));

	pTransform = new btDefaultMotionState(t);
	pShape->setLocalScaling(object->pScale);
	
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		pMass,              // mass, in kg. 
		pTransform,
		pShape,				// collision shape of body
		btVector3(0, 0, 0)	// local inertia
	);
	rigidBodyCI.m_restitution = 1;

	pBody = new btRigidBody(rigidBodyCI);
	pBody->setUserPointer(object);

	pWorld->addRigidBody(pBody);
}

void Decoration::Physics::update(RenderObject* obj, unsigned frame, float dTime)
{
	btTransform transform;
	pBody->getMotionState()->getWorldTransform(transform);
	glm::mat4 mat;
	transform.getOpenGLMatrix(glm::value_ptr(mat));
	Loggger::trace("Bullet: Updating transform of %s to %s", obj->name.c_str(), glm::to_string(mat).c_str());
	(*obj).transform = (pMass == 0.0f ? glm::mat4(1) : glm::mat4(0)) + mat;
}

void Decoration::Physics::onCollide(RenderObject* other)
{
}

Decoration::Physics::Physics(btDynamicsWorld* pWorld_, btCollisionShape* pShape_, float pMass_) : pShape(pShape_), pMass(pMass_), pWorld(pWorld_)
{
}


