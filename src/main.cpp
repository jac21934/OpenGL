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
		//   glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  

    
    Shader  modelShader("./shader_src/modelShader.vert", "./shader_src/modelShader.frag");
		Shader  lampShader("./shader_src/lampShader.vert", "./shader_src/lampShader.frag");
		Shader outlineShader("./shader_src/singleColorShader.vert", "./shader_src/singleColorShader.frag");
		
		Model testModel(std::string("/home/jacob/OpenGL/models/nanosuit/nanosuit.obj"));
//set up matrices for transforming the objects


		glm::vec3 lampPos;
		glm::vec3 sunDir = glm::vec3(0.0f, -1.0f, 0.0f);
		
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		modelShader.use();
		modelShader.setVec3("lampColor", color);
		
		
		Light dirLight("dirLight");
		dirLight.Set("direction", &sunDir);
		dirLight.Set("ambient", 0.2f, 0.2f, 0.2f);
		dirLight.Set("diffuse", 0.5f, 0.5f, 0.5f);
		dirLight.Set("specular", 1.0f, 1.0f, 1.0f);
		dirLight.Set("color", &color);


		Light pointLight("pointLight");
		pointLight.Set("position", &lampPos);
		pointLight.Set("ambient", 0.2f, 0.2f, 0.2f);
		pointLight.Set("diffuse", 0.5f, 0.5f, 0.5f);
		pointLight.Set("specular", 1.0f, 1.0f, 1.0f);
		pointLight.Set("color", &color);

		pointLight.Set("constant", 1.0f);
		pointLight.Set("linear", 0.09f);
		pointLight.Set("quadratic", 0.032f);
		


    while(!glfwWindowShouldClose(window)){
				glEnable(GL_DEPTH_TEST);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  

        //input
        processInput(window);
        //clear previous stuff
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				glStencilMask(0x00);

				glm::mat4 projection =  glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);  
				glm::mat4 view = camera.GetViewMatrix();
				
				

				//calulate lamp position
				lampPos = glm::vec3(0.5f*cos(glfwGetTime()),0.0f, 0.5f*sin(glfwGetTime()));

				glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should update the stencil buffer
				glStencilMask(0xFF); // 

			  glm::vec3 lampColor = glm::vec3(1.0f, 1.0f, 1.0f);
				lampShader.use();
				lampShader.setMat4("projection", projection);
				lampShader.setMat4("view", view);
				lampShader.setVec3("lightColor", lampColor);

				glm::mat4 lampModelMat;

				lampModelMat = glm::translate(lampModelMat, lampPos);
				lampModelMat = glm::scale(lampModelMat, glm::vec3(0.01f));
				lampShader.setMat4("model", lampModelMat);
				testModel.Draw(lampShader);


				
								




				//model
				modelShader.use();
				dirLight.Use(modelShader,0);
				pointLight.Use(modelShader,0);

				
				modelShader.setMat4("projection", projection);
				modelShader.setMat4("view", view);				
				modelShader.setVec3("viewPos", camera.Position);
				
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				modelShader.setMat4("model", model);								
				testModel.Draw(modelShader);

				model = glm::mat4();
				model = glm::translate(model, glm::vec3(0.0f, -1.75f, -10.0f));
				model = glm::scale(model, glm::vec3(1.5f));
				modelShader.setMat4("model", model);								
				testModel.Draw(modelShader);

				

//stencil outlining
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00); // disable writing to the stencil buffer
				glDisable(GL_DEPTH_TEST);
				outlineShader.use();

				
				model =  glm::mat4();
				model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
				model = glm::scale(model, glm::vec3(0.2f));
				model = glm::scale(model, glm::vec3(1.02f));
				outlineShader.setMat4("model", model);
				outlineShader.setMat4("view",  view);
				outlineShader.setMat4("projection", projection);
				testModel.Draw(outlineShader);

				model = glm::mat4();
				model = glm::translate(model, glm::vec3(0.0f, -1.75f, -10.0f));
				model = glm::scale(model, glm::vec3(1.5f));
				model = glm::scale(model, glm::vec3(1.02f));
				outlineShader.setMat4("model", model);
				outlineShader.setMat4("view",  view);
				outlineShader.setMat4("projection", projection);
				testModel.Draw(outlineShader);
				
				

        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);



				

				
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
