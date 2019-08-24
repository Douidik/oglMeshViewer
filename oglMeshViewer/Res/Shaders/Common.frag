#version 460 core
out vec4 color;

in vec2 frag_uv;
in vec3 out_pos;
in vec3 out_normals;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_height1;
uniform sampler2D texture_specular1;


void main()
{
    //Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    //Diffuse
    vec3 norm = normalize(out_normals);
    vec3 lightDir = normalize(lightPos - out_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - out_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor * vec3(texture(texture_specular1, frag_uv));


    //vec3 result
    vec3 texColor = vec3(texture(texture_diffuse1, frag_uv));
    vec3 result = (ambient + diffuse + specular) * texColor;
    color = vec4(result, 1.0);
}