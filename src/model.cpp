#include "model.h"


Model::Model(std::string path){

		loadModel(path);
}


void Model::loadModel(std::string path){
		Assimp::Importer import;
		
		const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


		
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
				std::string error = import.GetErrorString();
				std::cout << std::string("ERROR::ASSIMP::") + error << std::endl;
		}
		else{
				std::cout << "Model " + path + " imported." << std::endl
									<< "Processing " + path << std:: endl; 

				directory = path.substr(0, path.find_last_of('/'));
				processNode(scene->mRootNode, scene);

				std::cout << "Model " + path + " loaded." << std::endl;
				
		}
}

void Model::processNode(aiNode *node, const aiScene *scene){
		for(unsigned int i = 0; i < node->mNumMeshes; i++){
				aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
		}
		for(unsigned int i = 0; i <node->mNumChildren; i++){
				processNode(node->mChildren[i], scene);
		}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		float shine;
		for(unsigned int i = 0; i < mesh->mNumVertices; i++){

				Vertex vertex;
				glm::vec3 vec;
        vec.x = mesh->mVertices[i].x;				
				vec.y = mesh->mVertices[i].y;
				vec.z	= mesh->mVertices[i].z;
				vertex.Position = vec;

				vec.x = mesh->mNormals[i].x;				
				vec.y = mesh->mNormals[i].y;
				vec.z	= mesh->mNormals[i].z;
				vertex.Normal = vec;

				if(mesh->mTextureCoords[0]){
						glm::vec2 v2;
						v2.x = mesh->mTextureCoords[0][i].x;
						v2.y = mesh->mTextureCoords[0][i].y;
						vertex.TexCoords = v2;
				}
				else{
						vertex.TexCoords = glm::vec2(0.0f,0.0f);
				}

				vertices.push_back(vertex);
		}

		for(unsigned int i = 0; i < mesh->mNumFaces; i++){
				aiFace face = mesh->mFaces[i];
				for(unsigned int j = 0; j < face.mNumIndices;j++){
						indices.push_back(face.mIndices[j]);
				}
		}

		if(mesh->mMaterialIndex >= 0){
				aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
				//test
				if(AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shine) ){
						shine = 32.0;
				}
				std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "textures_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "textures_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		
		return Mesh(vertices, indices, textures, shine);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName){
		std::vector<Texture> tex;
		for(unsigned int i = 0; i < mat->GetTextureCount(type);  i++){
				aiString str;
				mat->GetTexture(type, i, &str);
				bool skip = false;
				for(unsigned int j = 0; j < textures_loaded.size(); j++){
						if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
								tex.push_back(textures_loaded[j]);
								skip = true;
								break;
						}
				}
				if(!skip){
						Texture texture;
						texture.id = TextureFromFile(str.C_Str(), directory);
						texture.type = typeName;
						texture.path = str.C_Str();
						tex.push_back(texture);
						textures_loaded.push_back(texture);
				}
		}
		return tex;
}


unsigned int Model::TextureFromFile(std::string file, std::string dir){

		stbi_set_flip_vertically_on_load(true);

		unsigned int ID;
		
		std::string textureFile = dir + std::string("/") + file;

		
		//   Type = "2D";
    int width, height, nrChannels;
    
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glGenTextures(1, &ID);


				GLenum format = GL_RGB;
				if(nrChannels == 1)
						format = GL_RED;
				else if(nrChannels == 3)
						format = GL_RGB;
				else if(nrChannels == 4)
						format = GL_RGBA;
				
        glBindTexture(GL_TEXTURE_2D, ID);				
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
        
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				std::cout <<"\t Texture " + textureFile + " loaded."  << std::endl;                
               
    }
    else{
        std::cout << "Failed to load texture " << textureFile  << std::endl;
    }
    stbi_image_free(data);

		return ID;

}

void Model::Draw(Shader shader){
		for(unsigned int i = 0; i < meshes.size(); i++){
//				std::cout << "Drawing mesh " + std::to_string(i) << std::endl;
				meshes[i].Draw(shader);
		}
}
				
		
