#version 330 core

struct Light{
		vec3 position;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
};

struct Material{
//		vec3 ambient;
		sampler2D diffuse;
		sampler2D specular;
		float shininess;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform Material material;
uniform Light light;

uniform vec3 objectColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
		// Geometry calculations
		vec3 norm       = normalize(Normal);
		vec3 lightDir   = normalize(light.position - FragPos);
		vec3 viewDir    = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		
		// Ambient Light
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
		
		// Diffuse Light
		float diff    = max(dot(norm, lightDir), 0.0);
		vec3  diffuse = light.diffuse * diff *  vec3(texture(material.diffuse, TexCoords));

		// Specular Lighting
		float spec     = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);
		vec3  specular = light.specular * spec *  vec3(texture(material.specular, TexCoords));

		// Result of all three lighting components
		vec3 result = (diffuse + ambient + specular) * objectColor;

		// Set the fragment color
    FragColor = vec4(result, 1.0);
}
