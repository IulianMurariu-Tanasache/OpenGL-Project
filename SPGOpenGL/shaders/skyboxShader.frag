#version 400 core
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 bloom;
in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    fragColor = texture(skybox, TexCoords);
    bloom = vec4(0.0f,0.0f,0.0f,1.0f);
}