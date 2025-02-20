#include <iostream>
#include <stdlib.h>
#include <string>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>


int screen_width{ 1080 };
int screen_height{ 1080 };

int num_frames{ 0 };
float last_time{ 0.0f };

float vertices[] =
{
	//    x      y      z   
		-1.0f, -1.0f, -0.0f,
		 1.0f,  1.0f, -0.0f,
		-1.0f,  1.0f, -0.0f,
		 1.0f, -1.0f, -0.0f
};

unsigned int indices[] =
{
	//  2---,1
	//  | .' |
	//  0'---3
		0, 1, 2,
		0, 3, 1
};

/* Window resize callback function prototype */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
/* Window input event callback function prototype */
void processInput(GLFWwindow* window);
/* FPS counter function prototype */
void countFPS();

int main()
{
	/* 
	* Initialize GLFW
	* 
	* GLFW is configured using glfwWindowHint.
	* glfwWindowHint takes in 2 parameters, the option, and the value.
	* option is selected from a large enum of possible options prefixed with 'GLFW_'.
	* value is an integer that sets the value of the option.
	* 
	* All options can be found here: https://www.glfw.org/docs/latest/window.html#window_hints
	*/
	glfwInit();
	// Set the target OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use core-profile mode without backwards-compatible features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create window object */
	GLFWwindow* window = glfwCreateWindow(800, 600, "Ying Xiang's Mandelbrot Viewer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* 
	* Initialize GLAD
	* 
	* GLAD manages function pointers for OpenGL, so we have to initialize it before
	* calling any OpenGL function.
	* 
	* GLFW defines glfwGetProcAddress for us, which is an OS-specific address of the
	* OpenGL function pointers.
	*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* Create Viewport */
	glViewport(0, 0, 800, 600);
	/* Register Callbacks */
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	last_time = glfwGetTime(); // Initialize last_time for FPS counting

	
	// Setup vertex data and buffers and configure vertex attributes
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO and copy vertices into buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	///////////////////////////
	//// START RENDER LOOP ////
	///////////////////////////
	while (!glfwWindowShouldClose(window))
	{
		// Process key inputs
		processInput(window);

		countFPS();

		// Core rendering commands
		// Clear buffer with specified background color
		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	///////////////////////////
	////  END RENDER LOOP  ////
	///////////////////////////

	/* Cleanup */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void countFPS()
{
	double current_time = glfwGetTime();
	num_frames++;
	if (current_time - last_time >= 1.0f)
	{
		std::cout << 1000.0f / num_frames << "ms / frames" << std::endl;
		num_frames = 0;
		last_time += 1.0f;
	}
}