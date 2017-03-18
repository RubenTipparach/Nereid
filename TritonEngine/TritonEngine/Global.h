#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
 const GLuint WIDTH = 800, HEIGHT = 600;

 // Deltatime
 GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
 GLfloat lastFrame = 0.0f;  	// Time of last frame

 bool keys[1024];
