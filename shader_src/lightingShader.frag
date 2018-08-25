#version 330 core
#define NR_POINT_LIGHTS 4
#define NR_DIFFUSE_TEXTURES 2
#define NR_SPECULAR_TEXTURES 3

struct PointLight{
		vec3 position;
		
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

		float constant;
		float linear;
		float quadratic;
};


struct SpotLight{
		vec3 position;
		vec3 direction;
		float cutOff;
		float outerCutOff;
		
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

		float constant;
		float linear;
		float quadratic;
};

struct DirLight{
		vec3 direction;
		
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

uniform vec3 objectColor;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse[NR_DIFFUSE_TEXTURES];
uniform sampler2D texture_specular[NR_SPECULAR_TEXTURES];

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;




out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{

		vec3 norm         = normalize(Normal);
		vec3 viewDir      = normalize(viewPos - FragPos);
		vec3 results = vec3(0.0, 0.0, 0.0);
		for (int i = 0; i < NR_POINT_LIGHTS; i++){
				results += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
		}
				FragColor = vec4(results, 1.0);
}



vec3 CalcPointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir){

		// Geometry calculations
		vec3 lightDir     = normalize(light.position - FragPos);
		vec3 reflectDir   = reflect(-lightDir, norm);
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		
		
		// Ambient Light
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));


		// Diffuse Light
		float diff    = max(dot(norm, lightDir), 0.0);
		vec3  diffuse = light.diffuse * diff *  vec3(texture(material.diffuse, TexCoords));

		// Specular Lighting
		float spec     = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);
		vec3  specular = light.specular * spec *  vec3(texture(material.specular, TexCoords));

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		return (ambient + diffuse + specular);
		
}



vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir){
		// Geometry calculations
		vec3 lightDir     = normalize(-light.direction);
		vec3 reflectDir   = reflect(-lightDir, norm);
				
		// Ambient Light
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));


		// Diffuse Light
		float diff    = max(dot(norm, lightDir), 0.0);
		vec3  diffuse = light.diffuse * diff *  vec3(texture(material.diffuse, TexCoords));

		// Specular Lighting
		float spec     = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);
		vec3  specular = light.specular * spec *  vec3(texture(material.specular, TexCoords));

		return (ambient + diffuse + specular);


}
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 FragPos, vec3 viewDir){

		// Geometry calculations
		vec3 lightDir     = normalize(light.position - FragPos);
		vec3 reflectDir   = reflect(-lightDir, norm);
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		
		//spotlight
		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);
		
		// Ambient Light
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));


		// Diffuse Light
		float diff    = max(dot(norm, lightDir), 0.0);
		vec3  diffuse = light.diffuse * diff *  vec3(texture(material.diffuse, TexCoords));

		// Specular Lighting
		float spec     = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);
		vec3  specular = light.specular * spec *  vec3(texture(material.specular, TexCoords));

		ambient  *= attenuation;
		diffuse  *= attenuation * intensity;
		specular *= attenuation * intensity;

		return (ambient + diffuse + specular);
		
}
