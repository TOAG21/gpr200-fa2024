#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "../core/cnh/Shader.h"
#include "../core/cnh/Texture.h"
#define STB_IMAGE_IMPLEMENTATION

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

	cnh::Shader logoShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	cnh::Shader artShader("assets/vertexShader.vert", "assets/fragmentShader (2).frag");

	float vertices[] = {
		//X      Y      Z	    R     G     B   U     V
		  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f
	};
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	
	//element buffer
	unsigned int EBO, VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//vertex buffer object - 0
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//link vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//vector color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	//uv coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	//texture work
	stbi_set_flip_vertically_on_load(true);
	cnh::Texture texture1 = cnh::Texture("assets/scpLogo.png", GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, 4);
	stbi_set_flip_vertically_on_load(true);
	cnh::Texture texture2 = cnh::Texture("assets/scp999.png", GL_NEAREST_MIPMAP_NEAREST, GL_CLAMP_TO_EDGE, 4);

	artShader.use();
	
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//logoscroll
		logoShader.setFloat("time", glfwGetTime());

		//Drawing happens here!
		glBindVertexArray(VAO);
		artShader.use();
		artShader.setInt("texture1", 0);
		artShader.setInt("texture2", 1);
		texture1.Bind(0);
		texture2.Bind(1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		logoShader.use();
		logoShader.setInt("texture1", 0);
		logoShader.setInt("texture2", 1);
		texture1.Bind(0);
		texture2.Bind(1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
