#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures){
		this->vertices = Vertices;
		this->indices  = Indices;
		this->textures = Textures;

		setupMesh();
}

void Mesh::setupMesh(){
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
		
}

void Mesh::Draw(Shader shader){
		unsigned int diffuseNR = 0;
		unsigned int specularNR = 0;
		for(unsigned int i = 0; i < textures.size(); i++){
				glActiveTexture(GL_TEXTURE0 + i);
				std::string number;
				std::string name = textures[i].type;
				if( name == "textures_diffuse"){
						number = std::to_string(diffuseNR++);
				}
				else if (name == "textures_specular"){
						number = std::to_string(specularNR++);
				}
				name = "material." + name+ "[" +  number + "]";
//				std::cout << name << std::endl;
								
				shader.setFloat((name  + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}


		//draw the mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
}
