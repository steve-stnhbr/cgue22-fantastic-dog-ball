#pragma once
#include <memory>
#include <typeindex>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btQuickprof.h>
#include <functional>

#include <unordered_map>

#include "Material.h"
#include "Render.h"

namespace Decoration
{
	class Decoration;
	class Physics;
	class Animation;
	class Custom;
};

/**
 * 
 */
class RenderObject
{
private:
	void applyToPhysics();
public:
	typedef std::list<RenderObject*> renderList;
	/**
	 * The Mesh containing the vertecies and indices used to draw
	 */
	Render::Mesh		mesh;
	/*
	 * A pointer to the material the RenderObject is using
	 */
	Material::Material*	material;
	/*
	 *
	 */
	Utils::Map<size_t, Decoration::Decoration*> decorations;
	/*
	 * This string is just for debugging purposes
	 */
	std::string			name;
	/**
	 * The index of the render object in the scene, set when adding object to the scene
	 */
	unsigned long long	index;
	/**
	 * unsigned 
	 */
	GLuint vaoID;
	/*
	 * matrix holding information about transforms
	 */
	glm::mat4 transform;
	btMatrix3x3 pRotate;
	btVector3 pTranslate;
	btVector3 pScale;

	RenderObject();
	RenderObject(Render::Mesh, Material::Material*, const std::string&);
	
	virtual void init();
	virtual void update(unsigned long frame, float dTime);
	virtual void draw(Shaders::Program prog);
	void add(Decoration::Decoration&);
	void add(Decoration::Decoration*);
	void buildVAO() const;
	virtual void cleanup();
	RenderObject* translate(float, float, float);
	RenderObject* translate(glm::vec3);
	RenderObject* rotate(float, float, float);
	RenderObject* rotate(float, glm::vec3);
	RenderObject* scale(float);
	RenderObject* scale(float, float, float);
	RenderObject* scale(glm::vec3);

	template <class T>
	inline T* getDecoration() const {
		auto& type = typeid(T);
		auto name = type.name();
		auto f = decorations.get(type.hash_code());
		if (f == NULL) return nullptr;
		return static_cast<T*>(f);
	}
};

namespace Decoration
{
	class Decoration
	{
	public:
		bool initialized;
		RenderObject* object;
		std::function<void(RenderObject*, unsigned long, float)> onUpdate;
		virtual void init(RenderObject*);
		virtual void update(RenderObject* object, unsigned frame, float dTime) = 0;

		void bind(RenderObject*);

		Decoration() = default;
		Decoration(RenderObject*);
	};


	class Physics : public Decoration
	{
	private:
		btCollisionShape* pShape;
		float pMass;
		btMotionState* pTransform;

	public:
		btRigidBody* pBody;
		btDynamicsWorld* pWorld;

		void init(RenderObject*) override;
		void update(RenderObject* object, unsigned frame, float dTime) override;

		void onCollide(RenderObject* other);

		/*
		 *	if pShape is nullptr the mesh of the object bound will be used
		 */
		Physics(btDynamicsWorld* pWorld, btCollisionShape* pShape, float mass);
	};

	class Animation : public Decoration
	{
	private:
		bool loop;
		float speed;
		std::vector<Render::Mesh> meshes;
		std::vector<std::string> paths;
	public:
		Animation();
		Animation(std::string path, float = 1.0f, bool = true);
		Animation(std::vector<std::string> paths, float = 1.0f, bool = true);

		void init(RenderObject*) override;
		void update(RenderObject*, unsigned frame, float dTime) override;
	};

	class Custom : public Decoration {
	public:
		std::function<void(RenderObject*, unsigned, float)> updateFunc;
		std::function<void(RenderObject*)> initFunc;

		Custom(std::function<void(RenderObject*, unsigned, float)> func);
		Custom(std::function<void(RenderObject*, unsigned, float)> update, std::function<void(RenderObject*)> init);

		void init(RenderObject*) override;
		void update(RenderObject*, unsigned frame, float dTime) override;
	};

}
