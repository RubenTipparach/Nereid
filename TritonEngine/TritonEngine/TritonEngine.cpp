// TritonEngine.cpp : Defines the entry point for the console application.
// https://learnopengl.com/
#include <iostream>
#include "stdafx.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Global.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Cube.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void Do_Movement();

// Shaders TODO: add these to shading scripts to load them at run time.

GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

/// ModelLoc -> model location pointer
void math_test(Shader* ourShader, float time, GLint* modelLoc)
{
	glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans;
	//trans = glm::translate(trans, glm::vec3(3.0f, 1.0f, 0.0f));
	//vec = trans * vec;

	//trans = glm::translate(trans, glm::vec3(0.0f, time* -0.1f, 0.0f));
	//trans = glm::rotate(trans, 0.1f * time, glm::vec3(0.0, 1.0, 1.0));

	//it uses radians now!! Also: L = T * R * S
	//trans = glm::translate(trans, glm::vec3(0.0f, time* -0.1f, 0.0f));
	//vec = trans * vec;
		
	//trans = glm::rotate(trans, glm::radians(10.0f)* time, glm::vec3(1.0, 1.0, 0.0)); 
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	//vec = trans * vec;

	//GLuint transformLoc = glGetUniformLocation(ourShader->Program, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

	//experimenting with perspective
	glm::mat4 model;
	model = glm::rotate(model, time*glm::radians(-55.00f), glm::vec3(1.0f, 1.0f, 0.0f));

	// Note that we're translating the scene in the reverse direction of where we want to move
	/*glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));*/
	//GLfloat radius = 10.0f;
	//GLfloat camX = sin(time) * radius;
	//GLfloat camZ = cos(time) * radius;

	//direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	//direction.y = sin(glm::radians(pitch));
	//direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	glm::mat4 view;
	//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));//changing the view matrix moves the camera.
	view = camera.GetViewMatrix();


	glm::mat4 projection;
	projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

	*modelLoc = glGetUniformLocation(ourShader->Program, "model");
	glUniformMatrix4fv(*modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewSpace = glGetUniformLocation(ourShader->Program, "view");
	glUniformMatrix4fv(viewSpace, 1, GL_FALSE, glm::value_ptr(view));

	GLint projSpace = glGetUniformLocation(ourShader->Program, "projection");
	glUniformMatrix4fv(projSpace, 1, GL_FALSE, glm::value_ptr(projection));
	//std::cout << vec.x << "," << vec.y << "," << vec.z << std::endl;
}

void drawAtLocation(Shader* ourShader, float time, glm::mat4 model)
{
	glm::mat4 view;
	view = camera.GetViewMatrix();

	glm::mat4 projection;
	projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

	GLint modelLoc = glGetUniformLocation(ourShader->Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewSpace = glGetUniformLocation(ourShader->Program, "view");
	glUniformMatrix4fv(viewSpace, 1, GL_FALSE, glm::value_ptr(view));

	GLint projSpace = glGetUniformLocation(ourShader->Program, "projection");
	glUniformMatrix4fv(projSpace, 1, GL_FALSE, glm::value_ptr(projection));
}

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triton Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);


	// Set the required callback functions
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_FRONT);


	//GLuint indices[] = {  // Note that we start from 0!
	//	0, 1, 3, // First Triangle
	//	1, 2, 3  // Second Triangle
	//};

	Shader litObjectShader("Shaders/lit_object.vs", "Shaders/standard_mat_shader.frag");
	Shader lampShader("Shaders/posonly_vertex.vs", "Shaders/color_frag.frag");

	GLuint VBO, VAO; //for lit object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWithNormals), verticesWithNormals, GL_STATIC_DRAW);
	glBindVertexArray(VAO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// TexCoord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	//################### Begin Light Shader stuff ###############################//

	GLuint lightVBO, lightVAO; //for the light
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	// We only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesNoTexCoords), verticesNoTexCoords, GL_STATIC_DRAW);
	glBindVertexArray(lightVAO);

	//verticesNoTexCoords

	// Set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//################### End Light Shader stuff    ###############################//

	// Uncommenting this call will result in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Load and create a texture 
	GLuint texture1;
	GLuint texture2;

	// auto destruct stackiness.
	{
		Texture loadTex1("Textures/container.jpg", &texture1);
		Texture loadTex2("Textures/awesomeface.png", &texture2);
	}

	//glm::mat4 proj = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	//glm::ortho(0.0f, 1200.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	// render loop!
	while (!glfwWindowShouldClose(window))
	{
		glm::vec3 lightPos = glm::vec3(1.2f, 0.1f , 2.0f); //camera.Position + camera.Front; //(1.2f, 0.1f * glfwGetTime(), 2.0f);

		//always calculate this first
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// check events
		glfwPollEvents();
		Do_Movement();

		// rendering commands
		glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

		//use that translation stuff
		//GLint modelLoc;
		//math_test(&ourShader, glfwGetTime(), &modelLoc);

		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // <--- only use with EBO
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// lighting stuff...
		// Use cooresponding shader when setting uniforms/drawing objects
		litObjectShader.Use();
		GLint lightPosLoc = glGetUniformLocation(litObjectShader.Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(litObjectShader.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		// Set lights properties
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence
		glUniform3f(glGetUniformLocation(litObjectShader.Program, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform3f(glGetUniformLocation(litObjectShader.Program, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
		glUniform3f(glGetUniformLocation(litObjectShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		// Set material properties
		glUniform3f(glGetUniformLocation(litObjectShader.Program, "material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(litObjectShader.Program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(litObjectShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
		glUniform1f(glGetUniformLocation(litObjectShader.Program, "material.shininess"), 32.0f);

		glm::mat4 model;
		model = glm::mat4();
		GLfloat angle = glm::radians(20.0f) * glfwGetTime();
		model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		drawAtLocation(&litObjectShader, glfwGetTime(),  model);


		lampShader.Use();

		
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		

		drawAtLocation(&lampShader, glfwGetTime(), model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		// Drawing and rotating those boxes.
		//for (GLuint i = 0; i < 10; i++)
		//{
		//	glm::mat4 model;
		//	model = glm::translate(model, cubePositions[i]);
		//	GLfloat angle = glm::radians(20.0f) * (i+1) * glfwGetTime();
		//	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		glBindVertexArray(0);

		// swap buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}