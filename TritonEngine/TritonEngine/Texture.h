#pragma once

#include <GL/glew.h>
#include <SOIL/SOIL.h>

class Texture
{
public:
	Texture(const char* filename, GLuint* texture);
	~Texture();
};

