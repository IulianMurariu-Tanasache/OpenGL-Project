#version 400 core
out vec4 FragColor;
  
in vec2 textCoord;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{             
    vec3 bloomColor = texture(bloomBlur, textCoord).rgb;
    vec3 hdrColor = texture(scene, textCoord).rgb;      
    if(bloomColor == vec3(0.0f,0.0f,0.0f))
    {
         FragColor = vec4(0,0,0, 1.0);
    }
    else{
        const float gamma = 2.2;
        hdrColor += bloomColor; // additive blending
        // tone mapping
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
       // result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
} 