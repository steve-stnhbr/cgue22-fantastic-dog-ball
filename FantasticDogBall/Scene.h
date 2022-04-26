#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <bullet/BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include <bullet/BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <bullet/LinearMath/btQuickprof.h>

#include "LightSource.h"
#include "Renderer.h"
#include "RenderObject.h"

/*
 * The scene class is responsible for handling all RenderObjects
 *
 */
class Scene
{
public:
	std::vector<RenderObject> objects;
	Light::Lights lights;
	Renderer renderer;

	Scene() = default;

	void addObject(RenderObject object);
	void addObject(RenderObject* object);

	void render();

	void setupPhysics();

protected:
	// core Bullet components
	btBroadphaseInterface* pBroadphase;
	btCollisionConfiguration* pCollisionConfiguration;
	btCollisionDispatcher* pDispatcher;
	btConstraintSolver* pSolver;
	btDynamicsWorld* pWorld;
	btClock clock;

	void addPhysics(btRigidBody*);
};