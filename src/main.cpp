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
#include "texture.h"

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


//Textures

    Texture texture("./textures/container2.png", "2D");
    Texture specular("./textures/container2_specular.png", "2D");

    cout << "Loaded textures" << endl;
//Shader Loading
    
    Shader lightingShader("./shader_src/lightingShader.vert", "./shader_src/lightingShader.frag");
    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

		lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setFloat("light.constant", 1.0f);
		lightingShader.setFloat("light.linear", 0.09f);
		lightingShader.setFloat("light.quadratic", 0.032f);
    Shader lampShader("./shader_src/lampShader.vert", "./shader_src/lampShader.frag");

    cout << "Loaded shaders." << endl;


    
//Vertex Buffers to pass to shaders
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
 

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);


    
    cout << "Bound buffers." << endl;
//set up matrices for transforming the objects
    glm::mat4 model;
    glm::mat4 lampModel;
    glm::mat4 view;
    glm::mat4 projection;


    projection =  glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);  
    glEnable(GL_DEPTH_TEST);    


    glm::vec3 lightPos(1.5f, 0.0f, 0.0f);


    
    
// Main loop    
    while(!glfwWindowShouldClose(window)){
        //input
        processInput(window);


        //clear previous stuff
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        
        // Calculate different matrices
        view = camera.GetViewMatrix();
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        lampModel = glm::mat4();
        lampModel = glm::translate(lampModel, lightPos);
        lampModel = glm::scale(lampModel, glm::vec3(0.2f)); 


        //light stuff
        lightPos = glm::vec3(1.5*cos(glfwGetTime()), 1.5*sin(glfwGetTime()), 0.0f);

        glm::vec3 lightColor;
        // lightColor.x = 0.7 * cos(glfwGetTime());
        // lightColor.x = sin(glfwGetTime());
        // lightColor.z = sin(glfwGetTime());

        glm::vec3 specularColor  =  glm::vec3(0.5f);
        
        //Lighting shader
        lightingShader.use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        
        lightingShader.setVec3("material.specular", specularColor);
        lightingShader.setFloat("material.shininess", 64.0f);       

        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
				lightingShader.setVec3("light.position", lightPos);
				lightingShader.setVec3("light.direction",-0.2, -0.1, -0.3);


				lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5)));
				lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5)));

				lightingShader.setVec3("viewPos", camera.Position);

				

        texture.Bind(GL_TEXTURE0);
        specular.Bind(GL_TEXTURE1);
        
        //update models
        for(int i = 0; i < 10; i++){
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
//            model = glm::rotate(model, (float)i*(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
        }


        // lightingShader.setMat4("model", model);
        // lightingShader.setVec3("lightPos", lightPos);
        // lightingShader.setVec3("viewPos", camera.Position);
        
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

				// Lamp shader
        lampShader.use();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("model", lampModel);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        //switch old stuff with new stuff and poll keyboard events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
