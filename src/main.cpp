#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>
#include <cmath>



//OpenGL includes
extern "C" {
#include <glad/glad.h>
}
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



//image loading
#include "stb_image.h"

//user made classes
#include "shapes.h"
#include "uselib.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));

bool firstMouse = true;
float lastX = 800.0f/2.0;
float lastY = 600.0f/2.0;
float fov   = 45.0f;
 		


float deltaTime = 0.0f;
float lastFrame = 0.0f;


glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};

int main(int argc, char** argv){

//stbi initialization
		stbi_set_flip_vertically_on_load(true);
	
// Initialization
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test Window", NULL, NULL);
		if( window == NULL){
				cout << "Failed to create GLFW window" << endl;
				glfwTerminate();
				return -1;
		}
		glfwMakeContextCurrent(window);

		//callback functions
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);


		//load GLAD
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
				cout << "Failed to initialize GLAD" << endl;
				return -1;
		}
		cout << "Initialized window." << endl;


		
//Shader Loading
		Shader shader("/home/jacob/OpenGL/shader_src/shader.vert","/home/jacob/OpenGL/shader_src/shader.frag" );

		cout << "Loaded shader." << endl;


		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);
 
				
		cout << "Bound buffers." << endl;
//textures

		Texture tex1("./textures/container.jpg", "2D");
		Texture tex2("./textures/SoilCracked.jpg", "2D");
				
		cout << "Loaded textures." << endl;

		//Load shaders once and then bind textures
		shader.use();
		shader.setInt("tex1", 0);
		shader.setInt("texture2", 1);

		
// Main loop

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;


		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));

		
		projection =  glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
				
		glEnable(GL_DEPTH_TEST);		

		

		
		while(!glfwWindowShouldClose(window)){
				//input
				processInput(window);


				//clear previous stuff
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Camera stuff
				view = camera.GetViewMatrix();
				
				//Give Matrics to shaders
				shader.setMat4("view", view);
				shader.setMat4("projection", projection);


				// textures;
				tex1.Bind(GL_TEXTURE0);
				tex2.Bind(GL_TEXTURE1);
				
				
				//use the shaders
				shader.use();

				glBindVertexArray(VAO);

				//update models
				for(int i = 0; i < 10; i++){
						glm::mat4 model;
						model = glm::translate(model, cubePositions[i]);
						float angle = 20.0f * i;
						model = glm::rotate(model, (float)i*(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
						model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
						shader.setMat4("model", model);
						glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				
				float currentFrame  = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;


				//switch old stuff with new stuff and poll keyboard events
				glfwSwapBuffers(window);
				glfwPollEvents();
		}
		
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glfwTerminate();
		return 0;
		
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
		glViewport(0,0,width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos){

		if (firstMouse){
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
}


void processInput(GLFWwindow* window){
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
				glfwSetWindowShouldClose(window, true);
		}
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
				camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
				camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
				camera.ProcessKeyboard(LEFT, deltaTime);
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
				camera.ProcessKeyboard(RIGHT, deltaTime);
		}
}
