//
// Created by samuel on 20/10/18.
//

#include "gl_utils.h"
#include <iostream>
#include <GL/glew.h>

using namespace std;

void checkGLError() {
	GLenum err;
	bool failed = false;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cout << "OpenGL Error : " << err << endl;
		failed = true;
	}
	if (failed)
		exit(0);
}