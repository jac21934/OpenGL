#ifndef MODEL_H
#define MODEL_H

#include "uselib.h"
#include "mesh.h"
#include "shader.h"

//image loading
#include "stb_image.h"


// assimp
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//normal includes
#include <iostream>
#include <string>
#include <vector>

//openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Model{
public:
		Model(std::string path);

		void Draw(Shader shader);

private:
		
		std::vector<Mesh> meshes;
		std::vector<Texture> textures_loaded;
		std::string directory;
		
		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

		unsigned int TextureFromFile(std::string file, std::string dir);


		
};


#endif
