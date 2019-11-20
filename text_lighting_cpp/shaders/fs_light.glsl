#version 330 core

out vec4 frag_color;

in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

uniform vec3 obj_color;
uniform vec3 view_pos;

uniform bool is_light; // Whether the obj shadered now is a light 


struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float near;
	float far;
};

struct FlashLight{
	vec3 position;
	vec3 direction;

	float cutoff;
	float cutoff_out;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float near;
	float far;

};

uniform Material material;
uniform PointLight point_light;
uniform FlashLight flash_light;
uniform DirectionalLight directional_light;

// calculates the color when using a directional light.
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coord));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float dist = length(light.position - fragPos);
	float attenuation=(dist-light.far)/(light.near-light.far);
	// combine
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,tex_coord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
    
    return ambient + diffuse + specular;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float dist = length(light.position - fragPos);
	float attenuation=(dist-light.far)/(light.near-light.far);
	// combine
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.cutoff_out;
    float intensity = clamp((theta - light.cutoff_out) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


void main(){
	if(is_light){
		//vec3 result=CalcPointLight(point_light,normal,frag_pos,normalize(view_pos - frag_pos));
		//frag_color=vec4(result,1.0f);
		frag_color=texture(material.diffuse,tex_coord);
	}
	else{
		vec3 norm = normalize(normal);
		vec3 viewDir = normalize(view_pos - frag_pos);
		vec3 result = vec3(0,0,0);
		result += CalcDirLight(directional_light, norm, viewDir);
		result += CalcPointLight(point_light, norm, frag_pos, viewDir);    
		//result += CalcSpotLight(flash_light, norm, frag_pos, viewDir);

		frag_color = vec4(result, 1.0);
	}
}
