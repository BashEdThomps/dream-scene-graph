/*
    Dream
    Shader Template: Environment Cube
    GLSL 330 Core
*/
#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube uEnvironmentMap;

void main()
{		
    vec3 envColor = textureLod(uEnvironmentMap, WorldPos, 0.0).rgb;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
}