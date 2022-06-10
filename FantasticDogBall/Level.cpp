#include "Level.h"

#include "glm/gtx/rotate_vector.hpp"
#include "LevelManager.h"

const glm::mat4 Level::rotateD = glm::rotate(glm::mat4(1), -glm::half_pi<float>(), { 0, 1, 0 });
const glm::mat4 Level::rotateA = glm::rotate(glm::mat4(1), glm::half_pi<float>(), { 0, 1, 0 });

Level::Level(Player* playerTemp, unsigned initialTime) : scene(), initialTime(initialTime), time(initialTime)
{
	setupPhysics();
	// copy value of player, so it does not have to be loaded again
	player = (Player*) malloc(sizeof(Player));
	memcpy(player, playerTemp, sizeof(Player));

	hud = new HUD();
	hud->init();
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
	player->init(pWorld);
	add(player->dog);
	add(player->ball);
	add(player);
}

State Level::render()
{
	float dt = clock.getTimeMilliseconds();
	// reset the clock to 0
	clock.reset();

	// check if the world object exists
	if (pWorld) {
		pWorld->stepSimulation(dt);
	}

	time -= dt / 1000;

	if (time < 0)
		return State::TIME_OVER;

	/*
	debugDrawer.debugProgram.use();
	debugDrawer.setCamera(scene.renderer.camera);
	*/
	scene.render(dt);
	if (player->ball->getDecoration<Decoration::Physics>()->pBody->getWorldTransform().getOrigin().y() < -20) return State::GAME_OVER;
	//pWorld->debugDrawWorld();
	Utils::start2D();
	hud->draw(std::to_string((int) time), std::to_string(bones));
	Utils::end2D();
	return LevelManager::state;
}

void physicsTick(btDynamicsWorld* world, btScalar timeStep);

void Level::cleanup()
{
	for (RenderObject* obj : scene.objects) {
		obj->cleanup();
	}
}

void physicsTick(btDynamicsWorld* world, btScalar timeStep);

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

void Level::remove(RenderObject* obj) {
	scene.removeObject(obj);
	pWorld->removeCollisionObject(obj->getDecoration<Decoration::Physics>()->pBody);
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

void Level::set(Cubemap* cubemap) {
	scene.cubemap = cubemap;
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
			Loggger::debug("%s collided with %s", rObjA->name.c_str(), rObjB->name.c_str());
			rObjA->getDecoration<Decoration::Physics>()->onCollide(rObjB);
			rObjB->getDecoration<Decoration::Physics>()->onCollide(rObjA);
		}
	}
}

void Level::addGravity(btVector3 vec) {
	pWorld->setGravity(pWorld->getGravity() + vec);
}

void Level::pressW()
{
	wGravity = glm::normalize(scene.renderer.camera.direction) * gravityMultiplier;
	addGravity({ wGravity.x, 0, wGravity.z});
	scene.renderer.camera.setPitch(.15);
}

void Level::pressA()
{
	aGravity = glm::rotateY(glm::vec4(glm::normalize(scene.renderer.camera.direction), 1),
		glm::radians<float>(90)) * gravityMultiplier;
	addGravity({ aGravity.x, 0, aGravity.z });
	scene.renderer.camera.setLeaning(.5);
}
void Level::pressS()
{
	sGravity = -glm::normalize(scene.renderer.camera.direction) * gravityMultiplier;
	addGravity({ sGravity.x, 0, sGravity.z });
	scene.renderer.camera.setPitch(-.15);
}
void Level::pressD()
{
	dGravity = glm::rotateY(glm::vec4(glm::normalize(scene.renderer.camera.direction), 1),
		glm::radians<float>(-90)) * gravityMultiplier;
	addGravity({ dGravity.x, 0, dGravity.z });
	scene.renderer.camera.setLeaning(.5);
}

void Level::releaseW() {
	addGravity({-wGravity.x, 0, -wGravity.z});
	wGravity = glm::vec3(0);
	scene.renderer.camera.setPitch(0);
}

void Level::releaseA() {
	addGravity({ -aGravity.x, 0, -aGravity.z });
	aGravity = glm::vec3(0);
	scene.renderer.camera.setLeaning(0);
}

void Level::releaseS() {
	addGravity({ -sGravity.x, 0, -sGravity.z });
	sGravity = glm::vec3(0);
	scene.renderer.camera.setPitch(0);
}

void Level::releaseD() {
	addGravity({ -dGravity.x, 0, -dGravity.z });
	dGravity = glm::vec3(0);
	scene.renderer.camera.setLeaning(0);
}

