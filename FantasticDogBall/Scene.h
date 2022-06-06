#pragma once
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <LinearMath/btQuickprof.h>

#include "LightSource.h"
#include "Renderer.h"
#include "RenderObject.h"

/*
 * The scene class is responsible for handling all RenderObjects
 *
 */
class Scene
{
public:
	typedef std::list<RenderObject> renderList;

	std::list<RenderObject> objects;
	Light::Lights lights;
	Renderer renderer;

	Scene() = default;

	void addObject(RenderObject object);
	void addObject(RenderObject* object);

	void render(float dTime);
};