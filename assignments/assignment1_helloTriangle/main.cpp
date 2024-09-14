#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "../core/cnh/Shader.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!

	cnh::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	float vertices[] = {
		//for context ive changed intial xy positions to the radians that the point sits at on a circle for easier roatation
      //radians x2    Z	    R     G     B     A
	    3.66f, 3.66f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		5.76f, 5.76f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.57f, 1.57f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
	};

	//vertex buffer object - 0
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	//link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//vector color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	

	float angles[] = {  210.0f, 330.0f, 90.0f };
	
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Drawing happens here!
		ourShader.use();

		//color fade and time input for rotation
		float time = glfwGetTime();
		float multiplier = (sin(time) / 2.0f) + 0.6f;
		ourShader.setFloat("colorMultiplier", multiplier);
		ourShader.setFloat("uTime", time);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
