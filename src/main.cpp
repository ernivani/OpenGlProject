
#define STB_IMAGE_IMPLEMENTATION    
#include "config.h"
#include "stb/stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

float mixVal= 0.5f;

glm::mat4 transform = glm::mat4(1.0f);
Joystick mainJ(0);


int main(void)	
{
	std::cout << "Hello World!" << std::endl;
	

	glfwInit();

	// opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);





	/*
		shaders
	*/

	Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");

	// vertex array
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.5f,	0.0f, 0.0f,
		-0.5f, 0.5f,  0.0f,		0.5f, 1.0f, 0.75f,  0.0f, 1.0f,
		0.5f,  -0.5f, 0.0f,		0.6f, 1.0f, 0.2f,	1.0f, 0.0f,
		0.5f,  0.5f,  0.0f,		1.0f, 0.2f, 1.0f,	1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		3, 1, 2 // second triangle
	};

	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// setup EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// set attributes pointers	

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// TEXTURES
	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// set texture filtering to GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("assets/image1.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);


	data = stbi_load("assets/image2.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}

	stbi_image_free(data);

	// shader


	shader.activate();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);


	glm::mat4 trans = glm::mat4(1.0f);
	// trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.activate();
	shader.setMat4("transform", trans);

	mainJ.update();
	if (mainJ.isPresent()) {
		std::cout << mainJ.getName() << " is present." << std::endl;
	}


	while (!glfwWindowShouldClose(window))
	{

		// process input
		ProcessInput(window);

		// render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		shader.activate();

		shader.setFloat("mixVal", mixVal);
		shader.setMat4("transform", transform);	

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

	// change mix value
	if (Keyboard::key(GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixVal += 0.0005f;
		if (mixVal >= 1.0f)
			mixVal = 1.0f;
	}

	if (Keyboard::key(GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixVal -= 0.0005f;
		if (mixVal <= 0.0f)
			mixVal = 0.0f;
	}

	if (Keyboard::key(GLFW_KEY_W) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, 0.001f, 0.0f));
	}

	if (Keyboard::key(GLFW_KEY_S) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, -0.001f, 0.0f));
	}

	if (Keyboard::key(GLFW_KEY_A) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(-0.001f, 0.0f, 0.0f));
	}

	if (Keyboard::key(GLFW_KEY_D) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(0.001f, 0.0f, 0.0f));
	}

	if (Keyboard::key(GLFW_KEY_Q) == GLFW_PRESS)
	{
		transform = glm::rotate(transform, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	if (Keyboard::key(GLFW_KEY_E) == GLFW_PRESS)
	{
		transform = glm::rotate(transform, glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	if (Keyboard::key(GLFW_KEY_R) == GLFW_PRESS)
	{
		transform = glm::scale(transform, glm::vec3(1.001f, 1.001f, 1.001f));
	}

	if (Keyboard::key(GLFW_KEY_F) == GLFW_PRESS)
	{
		transform = glm::scale(transform, glm::vec3(0.999f, 0.999f, 0.999f));
	}

	if (Keyboard::key(GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		transform = glm::mat4(1.0f);
	}

	mainJ.update();
}
