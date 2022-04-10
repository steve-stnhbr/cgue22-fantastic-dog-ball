
#include "Shaders.h"
// constexpr auto _ITERATOR_DEBUG_LEVEL = 2;

#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"

void error_callback(int error, const char* msg);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void initGl();
void initBullet();
void vertexArraySetup();

uint64_t frameCount = 0;

const int
	WINDOW_WIDTH = 1920,
	WINDOW_HEIGHT = 1080;
const float
	FOV = 45.0f;

struct TestBufferData
{
    glm::mat4 mvp;
    int isWireframe;
};

GLuint
/**
 * this holds the vertexArray ID
 */
    vao,
    /*
     * this holds our vertex buffer object
     */
    vbo;

int main(int argc, char* argv[])
{
    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit()) {
        fprintf(stderr, "GLFW-Error");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    initGl();
    initBullet();
    vertexArraySetup(); 

    constexpr float ratio = WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT);


    Scene scene = Scene();

    Render::StaticMaterial material = Render::StaticMaterial{};

    scene.addObject( RenderObject {
	    Render::Mesh {
	        std::vector<Vertex> {
	            Vertex {
	                {.5f, .5f, .0f},
	                {.0f, .0f, .0f},
	                {1.0f, .0f, .0f, 1.0f}
	            },
	            Vertex {
	                {.5f, -.5f, .0f},
	                {.0f, .0f, .0f},
	                {.0f, 1.0f, .0f, 1.0f}
	            },
	            Vertex {
	                {-.5f, .5f, .0f},
	                {.0f, .0f, .0f},
	                {.0f, .0f, 1.0f, 1.0f}
	            },
	        },
	        std::vector <unsigned> {
	            0, 1, 2
	        }
        }, &material, "TEST"
    });
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        frameCount++;
        fprintf(stdout, "Drawing frame #%llu\n", frameCount);
    }

    Shaders::cleanup();

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* msg) {
    fprintf(stderr, "(%i) %s", error, msg);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void initGl()
{
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
}

void initBullet() {
    auto const collisionConfiguration = new btDefaultCollisionConfiguration();
    auto const dispatcher = new btCollisionDispatcher(collisionConfiguration);
    auto const overlappingPairCache = new btDbvtBroadphase();
    auto const solver = new btSequentialImpulseConstraintSolver;
    auto const dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void vertexArraySetup() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}
