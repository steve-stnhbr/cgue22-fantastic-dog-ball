
// constexpr auto _ITERATOR_DEBUG_LEVEL = 2;

#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Loggger.h"
#include "Scene.h"
#include "Shaders.h"

#include "LightSource.h"
#include "Material.h"
#include "Level.h"
#include "RenderObject.h"
#include "Inputs.h"
#include "Player.h"
#include "LevelManager.h"
#include "UI.h"

#define NUM_LEVELS = 1

void error_callback(int error, const char* msg);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void initGl();
void initBullet();
void gl_error_callback(GLenum source​, GLenum type​, GLuint id​,
    GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam);
std::vector<std::string> getFirstNGLMessages(GLuint numMsgs);

const float
	FOV = 55.0f;
const char*
NAME = "Fantastic Dog Ball";


int main(int argc, char* argv[])
{
    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit()) {
        Loggger::fatal("Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT, NAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    Loggger::setLevel(Loggger::WARN);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, Inputs::keyCallback);

    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        Loggger::fatal("Error when initializing GLEW: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }
    Loggger::info("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    initGl();
    initBullet(); 

    LevelManager mgr;

    UI ui;
    ui.init();
    ui.display("");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Utils::checkError();
        
        mgr.render();

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
    Loggger::error("(%i) %s", error, msg);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void initGl()
{
	#ifdef FDB_DEBUG
		glEnable(GL_DEBUG_OUTPUT);

        glEnable(GL_DEBUG_OUTPUT);

        if (glDebugMessageCallbackARB != nullptr) {
            Loggger::debug("GL debug is available.\n");
            // Enable the debug callback
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(gl_error_callback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
                GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Started debugging");
        }
        else {
            Loggger::debug("GL debug is not available.\n");
        }

	#endif

    glEnable(GL_DEPTH_TEST);
    glDebugMessageCallback(gl_error_callback, nullptr); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void initBullet() {
    auto const collisionConfiguration = new btDefaultCollisionConfiguration();
    auto const dispatcher = new btCollisionDispatcher(collisionConfiguration);
    auto const overlappingPairCache = new btDbvtBroadphase();
    auto const solver = new btSequentialImpulseConstraintSolver;
    auto const dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void gl_error_callback(GLenum source​, GLenum type​, GLuint id​_, GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam)
{
    Loggger::log(severity​, "OpenGL-Log (%d): %s", id​_, message​);
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