//Normal includes
#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>
#include <cmath>


//OpenGL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//user made classes
#include "shapes.h"
#include "uselib.h"
#include "shader.h"
#include "camera.h"
#include "light.h"

//#include "texture.h"
#include "mesh.h"
#include "model.h"

using namespace std;

void windowInit(int major, int minor);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



Camera camera(glm::vec3(0.0f,0.0f,3.0f));

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

bool firstMouse = true;
float lastX = (float)SCR_WIDTH/2.0;
float lastY = (float)SCR_HEIGHT/2.0;
float fov   = 45.0f;
    
float deltaTime = 0.0f;
float lastFrame = 0.0f;




int main(int argc, char** argv){

// Initialization
    windowInit(3,3);
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
		glEnable(GL_DEPTH_TEST);    

    
    Shader  modelShader("./shader_src/modelShader.vert", "./shader_src/modelShader.frag");
		Shader  lampShader("./shader_src/lampShader.vert", "./shader_src/lampShader.frag");
		
		Model testModel(std::string("/home/jacob/OpenGL/models/nanosuit/nanosuit.obj"));
    Model lampModel(std::string("/home/jacob/OpenGL/models/sphere/sphere.obj"));

//set up matrices for transforming the objects

//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// Main loop

		glm::vec3 lampPos;
		glm::vec3 lampPos2; 
		glm::vec3 sunDir = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 sunDir2 = glm::vec3(0.0f, 1.0f, 0.0f);;
		
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 *pColor = &color;

		Light dirLight("dirLight");
		dirLight.Set("direction", &sunDir);
		dirLight.Set("ambient", 0.2f, 0.2f, 0.2f);
		dirLight.Set("diffuse", 0.5f, 0.5f, 0.5f);
		dirLight.Set("specular", 1.0f, 1.0f, 1.0f);
		dirLight.Set("color", pColor);


		Light dirLight2("dirLight");
		dirLight2.Set("direction", &sunDir2);
		dirLight2.Set("ambient", 0.2f, 0.2f, 0.2f);
		dirLight2.Set("diffuse", 0.5f, 0.5f, 0.5f);
		dirLight2.Set("specular", 1.0f, 1.0f, 1.0f);
		dirLight2.Set("color", pColor);

		modelShader.use();
		modelShader.setVec3("lampColor", color);
				

		
		dirLight.Values();
		dirLight2.Values();

		Light spotLight("spotLight");
		

    while(!glfwWindowShouldClose(window)){
        //input
        processInput(window);

//				lampPos = glm::vec3(0.5f*cos(glfwGetTime()),0.0f, 0.5f*sin(glfwGetTime()));

        //clear previous stuff
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				modelShader.use();
				modelShader.setVec3("viewPos", camera.Position);
				


				
				//model
				glm::mat4 projection =  glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);  
				glm::mat4 view = camera.GetViewMatrix();
				modelShader.setMat4("projection", projection);
				modelShader.setMat4("view", view);				
				
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				modelShader.setMat4("model", model);								


				dirLight.Use(modelShader, 0);
				dirLight2.Use(modelShader, 1);
				
				testModel.Draw(modelShader);

				
				glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwTerminate();
    return 0;
    
}



void windowInit(int major, int minor){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    float currentFrame  = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
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
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera.ProcessKeyboard(TURN_LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera.ProcessKeyboard(TURN_RIGHT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}
