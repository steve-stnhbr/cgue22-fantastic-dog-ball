#include "Items.h"

#include "LevelManager.h"

Items::DogTreat::DogTreat(btDynamicsWorld* world)
{}

Items::DogTreat::DogTreat(btDynamicsWorld* world, glm::vec3 position) : RenderObject(
	Render::Mesh::fromFile("../res/treat/treat.obj")[0],
	new Material::StaticMaterial({ .4f, .2f, .7f, 1.f }, .5f, .2f, 2, 1), // change to texture
	"DogTreat"
	)
{
	translate(position);
	/*
	auto mat = new Material::TextureMaterial();
	mat->color = { "../res/treat/color.png" };
	mat->normal = { "../res/treat/normal.png" };
	mat->diffuse = { 1.0f };
	mat->specular = { .1f };
	mat->shininess = .1f;
	*/
	auto physics = new Decoration::Physics(world, new btSphereShape(1), 0, [item = this](RenderObject* other) {
		LevelManager::current->remove(item);
		LevelManager::current->bones++;
	});

	auto custom = new Decoration::Custom([](RenderObject* obj, unsigned frame, float deltaTime) {
		obj->rotate(glm::radians(2.f), {0, 1, 0});
	});

	add(physics);
	add(custom);
}

void Items::DogTreat::onCollide(RenderObject* other)
{
	exists = false;
}


Items::Goal::Goal() : Goal({0,0,0})
{
}

Items::Goal::Goal(glm::vec3 position) : RenderObject(
	Render::Mesh::fromFile("../res/goal/goal.obj")[0],
	new Material::TextureMaterial(
		new Texture::Texture{ "../res/goal/color.png"},
		new Texture::Texture{ "" },
		new Texture::Texture{ .4f },
		new Texture::Texture{ .2f },
		new Texture::Texture{ .1f },
		1.4
	),
	"Goal"
)
{
	translate(position);

	auto custom = new Decoration::Custom([position = position](RenderObject* obj, unsigned frame, float deltaTime) {
		const auto playerPos = LevelManager::current->player->ball->getDecoration<Decoration::Physics>()
			->pBody->getWorldTransform().getOrigin();
		// if the player ball has the same position as the goal it is inside the goal
		if (glm::distance(glm::vec3(playerPos.x(), playerPos.y(), playerPos.z()), position) < .5) {
			LevelManager::state = State::LEVEL_FINISHED;
		}
	});

	add(custom);
}

Items::JumpPad::JumpPad(btDynamicsWorld* pWorld, glm::vec3 position = {0,0,0}, float strength = 20) : RenderObject(
	Render::Mesh::allFromFile("../res/jump_pad/jump_pad.obj"),
	new Material::TextureMaterial(
		new Texture::Texture("../res/jump_pad/color.png"),
		new Texture::Texture(""),
		new Texture::Texture("../res/jump_pad/diffuse.png"),
		new Texture::Texture("../res/jump_pad/specular.png"),
		new Texture::Texture("../res/jump_pad/reflectiveness.png"),
		1.2
	),
	"JumpPad"
), active(false), strength(strength) {
	translate(position);
	auto anim = new Decoration::Animation("../res/jump_pad/anim", .5f, false);
	anim->init(this);
	auto custom = new Decoration::Custom([strength = strength, active = active, anim = anim,position = position](RenderObject* obj, unsigned frame, float deltaTime) {
		const auto playerPos = LevelManager::current->player->ball->getDecoration<Decoration::Physics>()
			->pBody->getWorldTransform().getOrigin();
		// if the player ball has the same position as the goal it is inside the goal
		if (glm::distance(glm::vec3(playerPos.x(), playerPos.y(), playerPos.z()), position) < 2) {
			if (!active) {
				const auto test = LevelManager::current->player->ball->getDecoration<Decoration::Physics>()
					->pBody;

				test->setLinearVelocity(test->getLinearVelocity() * .7f + btVector3(0, strength, 0));
				/*
				LevelManager::current->player->ball->getDecoration<Decoration::Physics>()
					->pBody->applyCentralPushImpulse({ 0, 0, 0 });
				*/
				anim->reset();
				obj->add(anim);
				((JumpPad*)obj)->active = true;
			}
		}
	});
	add(custom);
}

