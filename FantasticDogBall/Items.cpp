#include "Items.h"


Items::DogTreat::DogTreat(btDynamicsWorld* world)
{

}

Items::DogTreat::DogTreat(btDynamicsWorld* world, glm::vec3 position)
{
	mesh = Render::Mesh::fromFile("../res/treat/treat.obj")[0];
	auto mat = new Material::TextureMaterial();
	mat->color = { "../res/treat/color.png" };
	mat->normal = { "../res/treat/normal.png" };
	mat->diffuse = { 1.0f };
	mat->specular = { .1f };
	mat->shininess = .1f;
	material = mat;
	name = "DogTreat";

	auto physics = new Decoration::Physics(world, nullptr, 0, [](RenderObject* other) {
		//TODO remove treat from level and increase score
	});
}

void Items::DogTreat::onCollide(RenderObject* other)
{
}
