#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <drawable/objvbo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define GLM_FORCE_RADIANS

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

int main(int argc, char** argv) {

    if (!glfwInit()) {
        fprintf(stderr, "Failed GLFW initialization\n");
        return -1;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window;
    window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    if(!window) {
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(error_callback);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    ObjVBO* objVBO = new ObjVBO("../res/models/super_ico.stl", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    glm::mat4 projectionMatrix = glm::frustum(-1.f , 1.f , -768.f / 1024.f , 768.f / 1024.f , 1.0f, 50.0f);
    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0., 0., -1.),
            glm::vec3(0., 0., 1.),
            glm::vec3(0,1,0)
    );

    glViewport(0, 0, 1024, 768);
    glClearColor(0.5, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    float angle = 0.0f;
    while (!glfwWindowShouldClose (window)) {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle+=1e-2, glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.f));
        glm::mat4 modelMatrix = translate * rotation;
        glm::mat4 mvMatrix =  viewMatrix * modelMatrix;
        glm::mat4 mvpMatrix = projectionMatrix * mvMatrix;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        objVBO->draw(mvpMatrix, mvMatrix, glm::vec3(0.0, 0.0, 0.0));
        glfwSwapBuffers (window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}