#pragma once
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <LinearMath/btQuickprof.h>

#include "Scene.h"

class Level
{
public:
	Scene scene;
	btDynamicsWorld* pWorld;
	
	Level();
	
	void render();

	void setupPhysics();
	void add(RenderObject);



protected:
	// core Bullet components
	btBroadphaseInterface* pBroadphase;
	btCollisionConfiguration* pCollisionConfiguration;
	btCollisionDispatcher* pDispatcher;
	btConstraintSolver* pSolver;
	btClock clock;

	void addPhysics(btRigidBody*);
};

