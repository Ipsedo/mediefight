#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include "shader.h"

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;

    GLFWwindow* window = glfwCreateWindow (500, 500, "OpenGL Frame", NULL, NULL);
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent (window);

    // Set key callback function
    /*glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, char_callback);*/

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit ();


    GLuint test = loadShader(GL_VERTEX_SHADER, "./res/shaders/diffuse_vs.glsl");

    std::cout<<test<<std::endl;
    return 0;
}