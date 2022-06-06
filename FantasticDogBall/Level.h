#pragma once
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletDynamics/ConstraintSolver/btConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <LinearMath/btQuickprof.h>

#include <glm/gtc/type_ptr.hpp>

#include "Player.h"
#include "Scene.h"

class Level
{
public:
	Scene scene;
	btDynamicsWorld* pWorld;
	Player* player;
	
	Level();
	~Level();
	
	void finalize();
	void render();

	void cleanup();
	void setupPhysics();
	void add(RenderObject&);
	void add(RenderObject*);
	void add(Light::Directional);
	void add(Light::Point);
	void add(Light::Spot);

protected:
	// core Bullet components
	btBroadphaseInterface* pBroadphase;
	btCollisionConfiguration* pCollisionConfiguration;
	btCollisionDispatcher* pDispatcher;
	btConstraintSolver* pSolver;
	btClock clock;

	void addPhysics(btRigidBody*);

	class BulletDebugDrawer_OpenGL : public btIDebugDraw {
		GLuint VBO, VAO;
		Camera camera;
	public:
		Shaders::Program debugProgram = Shaders::Program({ "debug.vert", "debug.frag" });
		inline void setCamera(Camera cam) {
			glUniformMatrix4fv(glGetUniformLocation(debugProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(cam.data.projection));
			glUniformMatrix4fv(glGetUniformLocation(debugProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(cam.data.view));
		}

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override
		{
			Loggger::debug("debug draw");
			// Vertex data
			GLfloat points[12];

			points[0] = from.x();
			points[1] = from.y();
			points[2] = from.z();
			points[3] = color.x();
			points[4] = color.y();
			points[5] = color.z();

			points[6] = to.x();
			points[7] = to.y();
			points[8] = to.z();
			points[9] = color.x();
			points[10] = color.y();
			points[11] = color.z();

			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glBindVertexArray(0);
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINES, 0, 2);
			glBindVertexArray(0);
		}
		virtual void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) {}
		virtual void reportErrorWarning(const char*) {}
		virtual void draw3dText(const btVector3&, const char*) {}
		virtual void setDebugMode(int p) {
			m = p;
		}
		int getDebugMode(void) const { return 1; }
		int m;
	} debugDrawer;
};

