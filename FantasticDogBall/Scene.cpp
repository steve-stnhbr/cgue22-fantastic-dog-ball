#include "Scene.h"


void Scene::addObject(RenderObject object)
{
	pWorld->addRigidBody(object.getCollision());
    object.index = objects.size();
	objects.push_back(object);

    // todo modify number óf light sources
}

void Scene::addObject(RenderObject* object)
{
    addObject(*object);
}

void Scene::render()
{
	float dt = clock.getTimeMilliseconds();
	// reset the clock to 0
	clock.reset();

	// check if the world object exists
	if (pWorld) {
		pWorld->stepSimulation(dt);
	}

    renderer.render(objects, lights);
}

void Scene::setupPhysics()
{
	// create the collision configuration
	pCollisionConfiguration = new btDefaultCollisionConfiguration();
	// create the dispatcher
	pDispatcher = new btCollisionDispatcher(pCollisionConfiguration);
	// create the broadphase
	pBroadphase = new btDbvtBroadphase();
	// create the constraint solver
	pSolver = new btSequentialImpulseConstraintSolver();
	// create the world
	pWorld = new btDiscreteDynamicsWorld(pDispatcher, pBroadphase, pSolver, pCollisionConfiguration);
}