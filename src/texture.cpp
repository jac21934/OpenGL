#include "texture.h"



Texture::Texture(std::string textureFile, std::string type){
    stbi_set_flip_vertically_on_load(true);

    Type = type;
    int width, height, nrChannels;
    std::string extension = getFileNameExtension(textureFile);
    
    data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        if(Type == "2D"){
            if( extension == std::string("jpg") || extension == std::string("jpeg")){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
            }
            else if (extension == std::string("png")){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            else{
                std::cout << "Unknown texture extension " << extension << std::endl;
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                
        }
        else if (Type == "1D"){
            //Nothing here yet
        }
        else if (Type == "3D"){
            //Nothing here yet
        }

    }
    else{
        std::cout << "Failed to load texture " << textureFile  << std::endl;
    }
    stbi_image_free(data);
}


void Texture::Bind(unsigned int OpenGLChannel){
    glActiveTexture(OpenGLChannel);
    if(Type == "2D"){
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    else if (Type == "1D"){
        //Nothing here yet
    }
    else if (Type == "3D"){
        //Nothing here yet
    }
}
