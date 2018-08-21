#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"

#include "uselib.h"

#include <iostream>
#include <glad/glad.h>
#include <string>

class Texture {
public:
//program ID
		unsigned int ID;

		//constructer
		Texture( std::string textureFile, std::string type );

		void Bind(unsigned int OpenGLChannel);
		
private:
//container for texture data
		unsigned char* data;

// Texture type, currently must be "2D"
		std::string Type;


};

#endif
