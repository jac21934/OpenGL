#ifndef MESH_H
#define MESH_H

#include "uselib.h"
#include "shader.h"

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct Texture{
		unsigned int id;
		std::string type;
		std::string path;
};

struct Vertex{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
};

class Mesh{
public:
		
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
		
		Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures);
		void Draw(Shader shader);

private:
		unsigned int VAO, VBO, EBO;

		void setupMesh();

		

};


#endif
