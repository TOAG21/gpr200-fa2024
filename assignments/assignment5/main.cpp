#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Inlcude IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "../core/cnh/Shader.h"
#include "../core/cnh/Texture.h"
#define STB_IMAGE_IMPLEMENTATION

void processInput(GLFWwindow* window);
void Fcallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCREEN_WIDTH / 2.0;
float lastY = SCREEN_HEIGHT / 2.0;
float fov = 60.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool perspectiveProjection = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cube Feild", NULL, NULL);
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

	cnh::Shader litShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	cnh::Shader lightCubeShader("assets/lightVShader.vert", "assets/lightFShader.frag");

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, Fcallback);
	glfwSetScrollCallback(window, scroll_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glEnable(GL_DEPTH_TEST);


	float vertices[] = {
//   X      Y     Z       U     V      Normal vector------
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};
	unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
	};
	glm::vec3 cubePositions[] = {
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f)),
	glm::vec3(ew::RandomRange(-8.0f, 8.0f),  ew::RandomRange(-8.0f, 8.0f), ew::RandomRange(-8.0f, 8.0f))
	};
	glm::vec3 cubeRotations[] = {
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)),
		glm::vec3(ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f), ew::RandomRange(0.0f, 1.2f)) //one extra vector so that they dont rotate the same direction they start in
	};
	float cubeScales[] = {
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f),
		ew::RandomRange(0.25f, 3.0f)
	};
	float rotationAngle = 20.0f;

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.2f;
	float specularStrength = 0.8f;
	float diffuseStrength = 1.0f;
	float shineStrength = 32.0f;
	float rotationSpeed = 0.5f;

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// location
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// vertex normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	litShader.use();
	//texture loading
	stbi_set_flip_vertically_on_load(true);
	cnh::Texture texture1 = cnh::Texture("assets/cube.png", GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, 4);

	litShader.setInt("texture1", 0);

	stbi_set_flip_vertically_on_load(true);
	cnh::Texture texture2 = cnh::Texture("assets/cubeNormal.png", GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, 4);

	litShader.setInt("texture2", 1);

	// glm look at - position, target, then up
	
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		processInput(window);
		//Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Drawing happens here!
		litShader.use();
		texture1.Bind(0);
		texture2.Bind(1);

		glm::mat4 view;
		view = glm::lookAt(
			cameraPos,
			cameraPos + cameraFront,
			cameraUp);
		glm::mat4 projection = glm::mat4(1.0f);
		if (perspectiveProjection)
			projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		else 
		{
			int orthoSize = 50;
			projection = glm::ortho(
				(float)-(SCREEN_WIDTH / orthoSize), (float)(SCREEN_WIDTH / orthoSize),
				(float)-(SCREEN_HEIGHT / orthoSize), (float)(SCREEN_HEIGHT / orthoSize),
				-5.0f, 500.0f);
		}
		// pass them to the shaders
		litShader.setMat4("view", view);
		litShader.setMat4("projection", projection);

		litShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.5f));
		litShader.setFloat("ambientStrength", ambientStrength);
		litShader.setFloat("specularStrength", specularStrength);
		litShader.setFloat("diffuseStrength", diffuseStrength);
		litShader.setFloat("shininess", shineStrength);
		litShader.setVec3("lightColor", lightColor);
		litShader.setVec3("lightPos", lightPos);
		litShader.setVec3("viewPos", cameraPos);

		glBindVertexArray(VAO);

		//make and rotate cubes
		for (unsigned int i = 0; i < 20; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(130.0f), cubeRotations[i]);
			model = glm::scale(model, glm::vec3(cubeScales[i]));

			rotationAngle += rotationSpeed * deltaTime;
			model = glm::rotate(model, glm::radians(rotationAngle),cubeRotations[i+1]);
			litShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//lightcube draw
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//imgui draw
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Settings");
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::ColorEdit3("LightColor", &lightColor.r);
		ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shineStrength, 2.0f, 1024.0f);
		ImGui::SliderFloat("Rotation", &rotationSpeed, 0.0f, 20.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = static_cast<float>(6.0f * deltaTime);
	else
		cameraSpeed = static_cast<float>(2.5f * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
}
void Fcallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		perspectiveProjection = !perspectiveProjection;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
	{
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}
	else 
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//veiw constraints
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	cameraUp = glm::cross(cameraFront, glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraFront)));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 120.0f)
		fov = 120.0f;
}

