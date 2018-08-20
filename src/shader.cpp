#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath){
		std::string vertexShaderString = getFileContents(vertexPath);
		std::string fragmentShaderString = getFileContents(fragmentPath);
		const char * vertexShaderSource = vertexShaderString.c_str();
		const char * fragmentShaderSource = fragmentShaderString.c_str();
		
		unsigned int vertexShader, fragmentShader;
		
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader,1,&vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		
		checkCompileErrors(vertexShader, "SHADER");

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, "SHADER");

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
}

void Shader::use(){
		glUseProgram(ID);
}


void Shader::setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name , float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x,y);
}

void Shader::setVec2(const std::string &name , glm::vec2 &value) const {
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name , float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}


void Shader::setVec3(const std::string &name , glm::vec3 &value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name , float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}


void Shader::setVec4(const std::string &name , glm::vec4 &value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



void Shader::checkCompileErrors(unsigned int shader, std::string type){
		int success;
		char infoLog[1024];
		if (type == "PROGRAM"){
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if(!success){
						glGetProgramInfoLog(shader, 1024, NULL, infoLog);
						std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl
											<< infoLog << std::endl;
				}
				
				
		}
		else if (type == "SHADER"){
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if(!success){
						glGetShaderInfoLog(shader, 1024, NULL, infoLog);
						std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl
											<< infoLog << std::endl;
				}
		}
}