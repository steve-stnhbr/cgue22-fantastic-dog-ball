
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
	vao;

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

    GLuint testBuf;
    glCreateBuffers(1, &testBuf);
    constexpr GLsizeiptr bufferSize = sizeof(TestBufferData);

    // glNamedBufferStorage(testBuf, bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    // glBindBufferRange(GL_UNIFORM_BUFFER, 0, testBuf, 0, bufferSize);

    constexpr float ratio = WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT);
    unsigned int program;

    try
    {
        program = Shaders::loadShadersFile(
            {
				GL_VERTEX_SHADER,
				GL_FRAGMENT_SHADER
            }, {
                "./VertexShader.glsl",
                "./FragmentShader.glsl"
            });
    }
    catch (Shaders::ShaderCompilationException& e)
    {
    	fprintf(stderr, "Shader compile error (%s):%s\n", e.shaderName.c_str(), e.what());
        glfwTerminate();
        exit(-12);
    }
    catch (Shaders::ProgramLinkException& e)
    {
        fprintf(stderr, "Program link error (%d):%s\n", e.program, e.what());
        glfwTerminate();
        exit(-13);
    }


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);


        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        /*
        const glm::mat4 m = glm::rotate(
            glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.5f)),
            static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f)
        );

        const glm::mat4 p = glm::perspective(FOV, ratio, 0.1f, 1000.0f);

        const TestBufferData buffer_data = {
            p * m,
            false
        };

        glNamedBufferSubData(
            testBuf, 0, bufferSize, &buffer_data
        );*/

        /* Render here */
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glLinkProgram(program);
        glUseProgram(program);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.0f, -1.0f);
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
