#include "Level.h"


Level::Level() : scene()
{
	setupPhysics();
}

Level::~Level()
{
	delete pWorld;
	delete pBroadphase;
	delete pCollisionConfiguration;
	delete pDispatcher;
	delete pSolver;
}

void Level::render()
{
	float dt = clock.getTimeMilliseconds();
	// reset the clock to 0
	clock.reset();

	// check if the world object exists
	if (pWorld) {
		pWorld->stepSimulation(dt);
	}

	scene.render(dt);
}

void Level::setupPhysics()
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
	pWorld->setGravity(btVector3(0, -9.81f, 0));

	

}

void Level::add(RenderObject obj)
{
	scene.addObject(obj);
	obj.init();
}
