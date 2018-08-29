#ifndef LIGHT_H
#define LIGHT_H

//openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.h"
#include "constants.h"



class Light{

public:
		std::string type;
		Light(std::string Type);

		void Set(std::string name, float value);
		void Set(std::string name, glm::vec3 value);
		void Set(std::string name, float x, float y, float z);
		void Set(std::string name, glm::vec3 *value);

		void Values();
		
		void Use(Shader shader);
		void Use(Shader shader, int index);

		
private:
		glm::vec3 *position;
		glm::vec3 *direction;
		glm::vec3 *color;
		
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;

		float cutOff;
		float outerCutOff;

		

};








#endif
