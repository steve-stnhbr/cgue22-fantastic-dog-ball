#pragma once
#include <memory>
#include <typeindex>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "Material.h"
#include "Render.h"
#include "HLMesh.h"


namespace Decoration
{
	class Decoration;
	class Physics;
	class Animation;
	class Compute;
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
	RenderObject* add(Decoration::Decoration&);
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
		virtual void onBind(RenderObject*) = 0;
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
		void onBind(RenderObject*) override;
	};

	class Animation : public Decoration
	{
	private:
		std::vector<Render::Mesh> meshes;
	public:
		void update(unsigned frame, float dTime) override;
		void onBind(RenderObject*) override;
	};

	class Compute : public Decoration {
	public:
		class ComputeType {
		private:
		protected:
			ComputeType() = default;
			ComputeType(Shaders::Program);
		public:
			unsigned short levels;
			bool isGeometry;
			Shaders::Program program;

			virtual void doCompute(RenderObject*) = 0;
		};
		class CatmullClarkSubdivision : public ComputeType {
		private:
		public:
			CatmullClarkSubdivision();
			CatmullClarkSubdivision(unsigned short levels);

			void doCompute(RenderObject*) override;
		};
		class SimpleSubdivision : public ComputeType {
		private:
			HLMesh subdivide(HLMesh);
			HLMesh subdivideAsync(HLMesh);
		public:
			SimpleSubdivision();
			SimpleSubdivision(unsigned short levels);

			void doCompute(RenderObject*) override;
		};

		std::vector<ComputeType*> types;

		Compute(ComputeType* type);
		Compute(std::vector<ComputeType*> types);

		void update(unsigned frame, float dTime) override;
		void onBind(RenderObject*) override;

		//Render::Mesh computeGeometry(ComputeType type, RenderObject* obj);
	};

}
