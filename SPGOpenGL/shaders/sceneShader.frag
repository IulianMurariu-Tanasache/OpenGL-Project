#version 400 core
out vec4 FragColor;
  
in vec2 textCoord;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{             
    vec3 bloomColor = texture(bloomBlur, textCoord).rgb;
    vec3 sceneColor = texture(scene, textCoord).rgb;      
    if(bloomColor == vec3(0.0f,0.0f,0.0f))
    {
         FragColor = texture(scene, textCoord);
    }
    else{
        sceneColor += bloomColor; // additive blending
        // tone mapping
        vec3 result = vec3(1.0) - exp(-sceneColor * exposure);
        FragColor = vec4(result, 1.0);
    }
} 