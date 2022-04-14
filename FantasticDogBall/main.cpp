
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
#include "Shaders.h"

#include "Render.h"

void error_callback(int error, const char* msg);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void initGl();
void initBullet();
void gl_error_callback(GLenum source​, GLenum type​, GLuint id​,
    GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam);
std::vector<std::string> getFirstNGLMessages(GLuint numMsgs);

const int
	WINDOW_WIDTH = 1920,
	WINDOW_HEIGHT = 1080;
const float
	FOV = 45.0f;
const char*
NAME = "Fantastic Dog Ball";

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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

    constexpr float ratio = WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT);


    Scene scene = Scene();

    Render::StaticMaterial material = Render::StaticMaterial{};

    std::vector<Vertex> vertecies = {
        Vertex{
            {.5f, .5f, .0f},
            {.0f, .0f, .0f},
            {1.0f, .0f, .0f, 1.0f}
        },
        Vertex{
            {1.0f, -.5f, .0f},
            {.0f, .0f, .0f},
            {.0f, 1.0f, .0f, 1.0f}
        },
        Vertex{
            {-.5f, .5f, .0f},
            {.0f, .0f, .0f},
            {.0f, .0f, 1.0f, 1.0f}
        },
    };

    std::vector <unsigned> indices = {
        0, 1, 2
    };

    scene.addObject(RenderObject{
        Render::Cube{
            0, 0, 0, 1, 1, 1
		}, &material, "CUBE"
    });

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Utils::checkError();

        scene.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        Utils::checkError();

        /* Poll for and process events */
        glfwPollEvents();
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
	for (auto msg : getFirstNGLMessages(10))
	{
        fprintf(stderr, (msg + "\n").c_str());
	} 
    fprintf(stderr, "(%i) %s", error, msg);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void initGl()
{
	#ifdef FTB_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
	#endif

    glEnable(GL_DEPTH_TEST);
    glDebugMessageCallback(gl_error_callback, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

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

void gl_error_callback(GLenum source​, GLenum type​, GLuint id​, GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam)
{
    fprintf(stderr, "Log (%d): %s\n", severity​, message​);
}

std::vector<std::string> getFirstNGLMessages(GLuint numMsgs)
{
    GLint maxMsgLen = 0;
    glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);

    std::vector<GLchar> msgData(numMsgs * maxMsgLen);
    std::vector<GLenum> sources(numMsgs);
    std::vector<GLenum> types(numMsgs);
    std::vector<GLenum> severities(numMsgs);
    std::vector<GLuint> ids(numMsgs);
    std::vector<GLsizei> lengths(numMsgs);

    GLuint numFound = glGetDebugMessageLog(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);

    sources.resize(numFound);
    types.resize(numFound);
    severities.resize(numFound);
    ids.resize(numFound);
    lengths.resize(numFound);

    std::vector<std::string> messages;
    messages.reserve(numFound);

    std::vector<GLchar>::iterator currPos = msgData.begin();
    for (size_t msg = 0; msg < lengths.size(); ++msg)
    {
        messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
        currPos = currPos + lengths[msg];
    }
    return messages;
}