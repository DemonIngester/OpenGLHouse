#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h" 


const unsigned int width = 800;
const unsigned int height = 800;

using namespace std;

//Shape Vertices
GLfloat  vertices[] = {

	//Coordinates                                   //Colors
	-0.5f,  0.0f,   0.25f,				1.0f, 0.0f, 1.0f,
	-0.5f,  0.0f,  -0.25f,				0.0f, 0.0f, 1.0f,
	 0.5f,  0.0f,  -0.25f,				1.0f, 0.0f, 0.0f,
	 0.5f,  0.0f,   0.25f,				0.0f, 1.0f, 0.0f,
	 0.0f,  0.4f,   0.0f,				1.0f, 0.0f, 0.0f
};

GLfloat houseVertices[] = {

	-0.5, -0.1,  0.25,                  1.0f, 0.0f, 0.0f,
	 0.5, -0.1,  0.25,					0.0f, 0.0f, 1.0f,
	 0.5,  0.1,  0.25,					1.0f, 0.0f, 0.0f,
	-0.5,  0.1,  0.25,					0.0f, 1.0f, 0.0f,

	-0.5, -0.1, -0.25,					0.0f, 0.0f, 1.0f,
	 0.5, -0.1, -0.25,					1.0f, 0.0f, 0.0f,
	 0.5,  0.1, -0.25,					0.0f, 1.0f, 0.0f,
	-0.5,  0.1, -0.25,					0.0f, 0.0f, 1.0f
};

GLuint houseIndices[] = {

	0, 1, 2,
	2, 3, 0,
	1, 5, 6,
	6, 2, 1,
	7, 6, 5,
	5, 4, 7,
	4, 0, 3,
	3, 7, 4,
	4, 5, 1,
	1, 0, 4,
	3, 2, 6,
	6, 7, 3
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main() {

	//Initalize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	GLFWwindow* window = glfwCreateWindow(width, height, "House by Kolton", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create window" << endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();


	glViewport(0, 0, width, height);

	//Load Shaders Source into Program
	Shader shaderProgram("default.vert", "default.frag");
	
	//Create Vertex Object
	VAO VAO1;
	VAO1.Bind();

	//Setup Buffers
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	//Link and Unbind Buffers
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//House Buffers
	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(houseVertices, sizeof(houseVertices));
	EBO EBO2(houseIndices, sizeof(houseIndices));

	//Link House Buffers
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	//Unbind House Buffers
	VBO2.Unbind();
	EBO2.Unbind();
	VAO2.Unbind();


	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	glEnable(GL_DEPTH_TEST);

	//Render Loop
	while (!glfwWindowShouldClose(window)) {


		glClearColor(0.25f, 0.10f, 0.33f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(25.00f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.25f, -2.0f));
		proj = glm::perspective(glm::radians(60.0f), (float)(width / height), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1f(uniID, 0.5f);
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		VAO1.Unbind();

		VAO2.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(houseIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers(window);



		glfwPollEvents();
	}

	//Delete arrays, buffers, and program after exiting loop
	
	
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}