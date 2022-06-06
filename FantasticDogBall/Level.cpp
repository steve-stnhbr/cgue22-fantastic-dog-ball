#include "Level.h"
#include "Player.h"


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

void Level::finalize() {
	player = new Player(pWorld);
	player->init();
	add(player->dog);
	add(player->ball);
	add(player);
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
	/*
	debugDrawer.debugProgram.use();
	debugDrawer.setCamera(scene.renderer.camera);
	*/
	scene.render(dt);
	//pWorld->debugDrawWorld();
}

void physicsTick(btDynamicsWorld* world, btScalar timeStep);

void Level::cleanup()
{
	for (RenderObject* obj : scene.objects) {
		obj->cleanup();
	}
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
	//pWorld->setDebugDrawer(&debugDrawer);

	pWorld->setInternalTickCallback(physicsTick);
}

void Level::add(RenderObject& obj)
{
	scene.addObject(obj);
	obj.init();
}

void Level::add(RenderObject* obj)
{
	scene.addObject(obj);
	obj->init();
}

void Level::add(Light::Directional l)
{
	scene.lights.add(l);
}
void Level::add(Light::Point l)
{
	scene.lights.add(l);
}
void Level::add(Light::Spot l)
{
	scene.lights.add(l);
}


void physicsTick(btDynamicsWorld* world, btScalar timeStep)
{
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		{
			const btCollisionObject* objA = contactManifold->getBody0();
			const btCollisionObject* objB = contactManifold->getBody1();
			const auto rObjA = static_cast<RenderObject*>(objA->getUserPointer());
			const auto rObjB = static_cast<RenderObject*>(objB->getUserPointer());
			Loggger::fatal("%s collided with %s", rObjA->name.c_str(), rObjB->name.c_str());
			rObjA->getDecoration<Decoration::Physics>()->onCollide(rObjB);
			rObjB->getDecoration<Decoration::Physics>()->onCollide(rObjA);
		}
	}
}
