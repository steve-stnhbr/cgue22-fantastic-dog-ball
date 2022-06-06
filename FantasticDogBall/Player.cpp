#include "Player.h"

Player::Player(btDynamicsWorld* pWorld) : Player(pWorld, {0,0,0})
{
}

Player::Player(btDynamicsWorld* pWorld, glm::vec3 position)
{
	auto* ball_mat = new Material::StaticMaterial({ .8, .8, .9, .76 }, { .32 }, { .8 }, 7.4);
	ball = new RenderObject{ Render::Sphere(1, 16, 32), ball_mat, "PlayerBall" };
	auto* physics = new Decoration::Physics(pWorld, new btSphereShape(1), 20);
	ball->add(physics); 

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
	auto velocity = ball->getDecoration<Decoration::Physics>()->pBody->getLinearVelocity();
	velocity.setY(0);

	ball->update(frame, dTime);
	dog->update(frame, dTime);
	directionAngle = glm::atan(velocity.x() / velocity.z());
	dog->transform = glm::rotate(glm::translate(ball->transform, { 0, -.6, 0 }), directionAngle, {0, 1, 0});

	const auto speed = velocity.length2();

	if (speed > 17)
		dog->add(canter);
	else if (speed > 9)
		dog->add(trot);
	else if (speed > .5)
		dog->add(walk);
	else
		dog->add(stand);
}

void Player::draw(Shaders::Program prog)
{
}
