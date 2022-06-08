#include "Player.h"

#include "LevelManager.h"

Player::Player() : Player({0,0,0})
{
}

Player::Player(glm::vec3 position)
{
	auto* dog_material = new Material::TextureMaterial();
	dog_material->color = { "../res/dog/color.png" };
	dog_material->normal = { "../res/dog/normal.png" };
	dog_material->shininess = .8;
	dog_material->specular = { .5 };
	dog = new RenderObject{ Render::Mesh::fromFile("../res/dog/dog.obj")[0], dog_material, "PlayerDog" };

	stand = new Decoration::Animation(std::vector<std::string>{ "../res/dog/dog.obj" }, 0, false);
	stand->init(dog);
	walk = new Decoration::Animation("../res/dog/dog_walk", .6);
	walk->init(dog);
	trot = new Decoration::Animation("../res/dog/dog_trot", .6);
	trot->init(dog);
	canter = new Decoration::Animation("../res/dog/dog_canter", .6);
	canter->init(dog);

	//ball->init(); 
	//dog->init();
}

void Player::update(unsigned long frame, float dTime)
{
	// get information about players state
	auto ballBody = ball->getDecoration<Decoration::Physics>()->pBody;
	auto velocity = glm::vec3(ballBody->getLinearVelocity().x(), 0, ballBody->getLinearVelocity().z());

	const auto speed = glm::length(velocity);
	velocity = glm::normalize(velocity);

	// change animation based on the size of the velocity
	if (speed > 17)
		dog->add(canter);
	else if (speed > 9)
		dog->add(trot);
	else if (speed > .5)
		dog->add(walk);
	else
		dog->add(stand);

	// set transformations of player-ball and dog to the same
	ball->update(frame, dTime);
	dog->update(frame, dTime);
	directionAngle = glm::atan(velocity.x / velocity.z);

	if (directionAngle != directionAngle) directionAngle = 0;
	dog->transform = glm::rotate(glm::translate(ball->transform, { 0, -.6, 0 }), directionAngle, { 0, 1, 0 });

	// change camera to follow player

	const auto oldCamDirection = LevelManager::current->scene.renderer.camera.direction;
	const auto oldCamAngle = glm::atan(oldCamDirection.x / oldCamDirection.z);
	const auto diffAngle = directionAngle - oldCamAngle;
	auto newCamAngle = .0;
	if (abs(diffAngle) < .05)
		newCamAngle = directionAngle;
	else
		newCamAngle = oldCamAngle + diffAngle * .1;
	const auto ballPos = ballBody->getWorldTransform().getOrigin();
	LevelManager::current->scene.renderer.camera.setYaw(newCamAngle);
	LevelManager::current->scene.renderer.camera.setPosition(glm::vec3(ballPos.x(), ballPos.y(), ballPos.z()));
	LevelManager::current->scene.renderer.camera.update();
}

void Player::draw(Shaders::Program prog)
{ 
	// here goes nothing
}

void Player::init()
{
	RenderObject::init();
}

void Player::init(btDynamicsWorld* pWorld)
{
	auto* ball_mat = new Material::StaticMaterial({ .8, .8, .9, .76 }, { .32 }, { .8 }, 7.4, .4);
	ball = new RenderObject{ Render::Sphere(1, 16, 32), ball_mat, "PlayerBall" };
	auto* physics = new Decoration::Physics(pWorld, new btSphereShape(1), 20);
	ball->add(physics);
}

