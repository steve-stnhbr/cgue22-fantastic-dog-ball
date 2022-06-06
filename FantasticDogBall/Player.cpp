#include "Player.h"

Player::Player(Level& level) : Player(level, {0,0,0})
{
}

Player::Player(Level& level, glm::vec3 position)
{
	auto* ball_mat = new Material::StaticMaterial({ .6, .6, .8, .4 }, { .32 }, { .4 }, 2.4);
	ball = RenderObject{ Render::Sphere(1, 16, 32), ball_mat, "PlayerBall" };
	auto* physics = new Decoration::Physics(level.pWorld, nullptr, 20);
	ball.add(physics); 

	auto* dog_material = new Material::TextureMaterial();
	dog_material->color = { "../res/dog/color.png" };
	dog_material->normal = { "../res/dog/normal.png" };
	dog_material->shininess = .8;
	dog_material->specular = { .5 };
	dog = RenderObject{ Render::Mesh::fromFile("../res/dog/dog.obj")[0], dog_material, "PlayerDog" };

	stand = Decoration::Animation(std::vector<std::string>{ "../res/dog/dog.obj" }, 0, false);
	stand.init(&dog);
	walk = Decoration::Animation("../res/dog/dog_walk", .6);
	walk.init(&dog);
	trot = Decoration::Animation("../res/dog/dog_trot", .6);
	trot.init(&dog);
	canter = Decoration::Animation("../res/dog/dog_canter", .6);
	canter.init(&dog);

	ball.init();
	dog.init();
}

void Player::update(unsigned long frame, float dTime)
{
	ball.update(frame, dTime);
	dog.update(frame, dTime);
	dog.transform = glm::translate(ball.transform, {0, -.5, 0});

	auto velocity = ball.getDecoration<Decoration::Physics>()->pBody->getLinearVelocity();
	velocity.setY(0);
	const auto speed = velocity.length2();

	if (speed > 17)
		dog.add(canter);
	else if (speed > 9)
		dog.add(trot);
	else if (speed > 2)
		dog.add(walk);
	else
		dog.add(stand);
	

}

void Player::draw(Shaders::Program prog)
{
}
