#include "light.h"


Light::Light(std::string Type){
		type = Type;
		
		position = &const_cast<glm::vec3&>(ZERO_VEC3);
		direction = &const_cast<glm::vec3&>(ZERO_VEC3);
		color = &const_cast<glm::vec3&>(COLOR_WHITE);
//		color = (&const_cast<glm::vec3>(COLOR_WHITE)) ;
		
		ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		specular = glm::vec3(0.0f, 0.0f, 0.0f);

		constant    = 0.0f;
		linear      = 0.0f;
		quadratic   = 0.0f;

		cutOff      = 0.0f;
		outerCutOff = 0.0f;
}

void Light::Values(){
		std::cout << "Type:\t\t" << type << std::endl
							<< "Position:\t"     <<	  std::to_string((*position)[0])   << " " << std::to_string((*position)[1])   << " " << std::to_string((*position)[2])  << std::endl
							<< "Direction:\t"    <<		std::to_string((*direction)[0])  << " " <<  std::to_string((*direction)[1]) << " " << std::to_string((*direction)[2]) << std::endl
							<< "Color:\t\t"        <<		std::to_string((*color)[0])      << " " <<  std::to_string((*color)[1])     << " " << std::to_string((*color)[2])     << std::endl

				
							<< "Ambient:\t"      <<		std::to_string(ambient.x) << " "  << std::to_string(ambient.y)  << " " << std::to_string(ambient.z)    << std::endl
							<< "Diffuse:\t"      <<		std::to_string(diffuse.x) << " "  << std::to_string(diffuse.y)  << " " << std::to_string(diffuse.z)    << std::endl
							<< "Specular:\t"     <<		std::to_string(specular.x)<< " "  << std::to_string(specular.y) << " " << std::to_string(specular.z)   << std::endl

							<< "Constant:\t"     <<		std::to_string(constant)     << std::endl
							<< "Linear:\t\t"       <<		std::to_string(linear)       << std::endl
							<< "Quadratic:\t"    <<		std::to_string(quadratic)    << std::endl
							<< "Cutoff:\t\t"       <<   std::to_string(cutOff)       << std::endl
							<< "Outer Cutoff:\t" <<	  std::to_string(outerCutOff)  << std::endl;

}

void Light::Set(std::string name, float value){
		if( name == "constant")
				constant = value;
		else if( name == "linear")
				linear = value;
		else if( name == "quadratic")
				quadratic = value;
		else if( name == "cutOff")
				cutOff = value;
		else if( name == "outerCutOff")
				outerCutOff = value;
}

void Light::Set(std::string name, glm::vec3 value){
		if(name == "ambient")
				ambient = value;
		else if(name == "diffuse")
				diffuse = value;
		else if(name == "specular")
			specular = value;
		else if ( (name == "color") || (name == "position") || (name == "direction")){

				std::cout << name + " must be initialized using a pointer." << std::endl;
		}
		else{
				std::cout << "Unrecognized parameter: " + name << std::endl;
		}
}

void Light::Set(std::string name, glm::vec3 *value){
		if(name == "color")
				color = value;
		else if(name == "position")
				position = value;
		else if(name == "direction")
				direction = value;

		
}


void Light::Set(std::string name, float x, float y, float z){
		glm::vec3 value = glm::vec3(x,y,z);
		Set(name, value);
		
}


void Light::Use(Shader shader){
		int index = -1;
		Use(shader, index);
}

void Light::Use(Shader shader, int index){
		std::string name = type;

		if( index >= 0){
				name += "[" + std::to_string(index) + "]";
		}

		shader.setVec3((name + ".ambient").c_str(), ambient);
		shader.setVec3((name + ".diffuse").c_str(), diffuse);
		shader.setVec3((name + ".specular").c_str(), specular);
		shader.setVec3((name + ".color").c_str(), (*color));
//		shader.setVec3((name + ".color").c_str(), (*color)[0], (*color)[1], (*color)[2]);
		
		
				
		
		if(type == "pointLight" || type == "spotLight"){
				shader.setVec3((name + ".position").c_str(), (*position));
				
				shader.setFloat((name + ".constant").c_str(), constant);
				shader.setFloat((name + ".linear").c_str(), linear);
				shader.setFloat((name + ".quadratic").c_str(), quadratic);
				if(type == "spotLight"){
						shader.setVec3((name + ".direction").c_str(), (*direction));
								
						shader.setFloat((name + ".cutOff").c_str(), cutOff);
						shader.setFloat((name + ".cutOff").c_str(), cutOff);
						shader.setFloat((name + ".outerCutOff").c_str(), outerCutOff);
				}
		}
		else if(type == "dirLight"){
				shader.setVec3((name + ".direction").c_str(), (*direction));
		}
		
}




		
