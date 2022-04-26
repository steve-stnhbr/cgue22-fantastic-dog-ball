#pragma once
#include <memory>
#include <typeindex>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "Material.h"
#include "Render.h"


namespace Decoration
{
	class Decoration;
	class Physics;
	class Animation;
};

/**
 * 
 */
class RenderObject
{
public:
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
	// std::unordered_map<std::type_index, Decoration::Decoration> decorations;
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
	GLuint vaoID, vboID, eboID;
	/*
	 * matrix holding information about transforms
	 */
	glm::mat4 transform;

	RenderObject(Render::Mesh, Material::Material*, const std::string&);
	
	void update();
	void add(Decoration::Decoration&);
	void buildVAO() const;
	RenderObject* translate(float, float, float);
	RenderObject* translate(glm::vec3);
	RenderObject* rotate(float, float, float);
	RenderObject* rotate(float, glm::vec3);
	RenderObject* scale(float, float, float);
	RenderObject* scale(glm::vec3);

private:
	void doTransform();
};

class Cube : public RenderObject
{
	Cube(float centerX, float centerY, float centerZ, float width, float height, float depth);
};

namespace Decoration
{
	class Decoration
	{
	protected:
		RenderObject* object;
	public:
		virtual void update(unsigned frame, float dTime) = 0;

		void bind(RenderObject*);
	};


	class Physics : public Decoration
	{
	private:
		btCollisionShape* pShape;
		float mass;
		btMotionState* motionState;
	public:
		void update(unsigned frame, float dTime) override;
	};

	class Animation : public Decoration
	{
	private:
		std::vector<Render::Mesh> meshes;
	public:
		void update(unsigned frame, float dTime) override;
	};

}
