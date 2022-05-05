#pragma once
#include <memory>
#include <typeindex>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btQuickprof.h>

#include <unordered_map>

#include "Material.h"
#include "Render.h"


namespace Decoration
{
	class Decoration;
	class Physics;
	class Animation;
};

/**
 * 
 */
class RenderObject
{
public:
	/**
	 * The Mesh containing the vertecies and indices used to draw
	 */
	Render::Mesh		mesh;
	/*
	 * A pointer to the material the RenderObject is using
	 */
	Material::Material*	material;
	/*
	 *
	 */
	std::unordered_map<const char*, Decoration::Decoration*> decorations;
	/*
	 * This string is just for debugging purposes
	 */
	std::string			name;
	/**
	 * The index of the render object in the scene, set when adding object to the scene
	 */
	unsigned long long	index;
	/**
	 * unsigned 
	 */
	GLuint vaoID, vboID, eboID;
	/*
	 * matrix holding information about transforms
	 */
	glm::mat4 transform;
	btMatrix3x3 pRotate;
	btVector3 pTranslate;
	btVector3 pScale;

	RenderObject(Render::Mesh, Material::Material*, const std::string&);
	
	void init();
	void update(unsigned long frame, float dTime);
	void add(Decoration::Decoration&);
	void buildVAO() const;
	RenderObject* translate(float, float, float);
	RenderObject* translate(glm::vec3);
	RenderObject* rotate(float, float, float);
	RenderObject* rotate(float, glm::vec3);
	RenderObject* scale(float);
	RenderObject* scale(float, float, float);
	RenderObject* scale(glm::vec3);

private:
	void doTransform();
};

namespace Decoration
{
	class Decoration
	{
	protected:
		RenderObject* object;
	public:
		virtual void init(RenderObject*) = 0;
		virtual void update(RenderObject* object, unsigned frame, float dTime) = 0;

		void bind(RenderObject*);

		Decoration() = default;
	};


	class Physics : public Decoration
	{
	private:
		btCollisionShape* pShape;
		float pMass;
		btMotionState* pTransform;
		btDynamicsWorld* pWorld;

		btRigidBody* pBody;
	public:
		void init(RenderObject*) override;
		void bind(RenderObject*);
		void update(RenderObject* object, unsigned frame, float dTime) override;

		/*
		 *	if pShape is nullptr the mesh of the object bound will be used
		 */
		Physics(btDynamicsWorld* pWorld, btCollisionShape* pShape, float mass);
	};

	class Animation : public Decoration
	{
	private:
		std::vector<Render::Mesh> meshes;
	public:
		void init(RenderObject*) override;
		void update(RenderObject* object, unsigned frame, float dTime) override;
	};

}
