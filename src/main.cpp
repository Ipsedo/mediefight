#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <drawable/objvbo.h>
#include <ext.hpp>
#include "shader.h"

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;

    if (!glfwInit()) {
        fprintf(stderr, "Failed GLFW initialization\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // On veut OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Pour rendre MacOS heureux ; ne devrait pas être nécessaire
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window; // (Dans le code source qui accompagne, cette variable est globale)
    window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialise GLEW
    glewExperimental=true; // Nécessaire dans le profil de base
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    GLuint test = loadShader(GL_VERTEX_SHADER, "../res/shaders/diffuse_vs.glsl");

    ObjVBO* objVBO = new ObjVBO("../res/obj/icosahedron.obj");

    glm::mat4 projectionMatrix = glm::frustum(-0.0512f , 0.0512f , -0.0384f , 0.0384f , 0.1f, 50.0f);
    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0., 0., -1.), // Camera  in World Space
            glm::vec3(0., 0., 1.), // and looks at look at
            glm::vec3(0,0,1)  // Head is up (set to 0,0,1)
    );
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glViewport(0, 0, 1024, 768);
    glClearColor(0., 0., 0., 1.);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    while (!glfwWindowShouldClose (window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 mvMatrix = viewMatrix * modelMatrix;
        glm::mat4 mvpMatrix = projectionMatrix * mvMatrix;
        objVBO->draw(mvpMatrix, mvMatrix, glm::vec3(0.0, 0.0, -1.0));
    }


    std::cout<<test<<" : valeur shader"<<std::endl;
    return 0;
}