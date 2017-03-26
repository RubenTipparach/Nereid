#pragma once
#include <iostream>
#include "stdafx.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
	glm::vec3 localPosition;
	glm::vec3 globalPosition;

	glm::vec3 scale;

	glm::vec3 eulerRotation;

	Transform();
	~Transform();
};

