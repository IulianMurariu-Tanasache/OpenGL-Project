#version 400
out vec4 FragColor;
  
in vec2 textCoord;

#define SAMPLE_SIZE 7
#define MEAN_OFFSET 0.117f

uniform sampler2D image;
  
uniform bool horizontal;
//uniform float weight[SAMPLE_SIZE] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform float weight[SAMPLE_SIZE] = {
    0.2762540 - MEAN_OFFSET,
    0.1950520 - MEAN_OFFSET,
    0.2018310 - MEAN_OFFSET,
    0.1893790 - MEAN_OFFSET,
    0.2570880 - MEAN_OFFSET,
    0.1678060 - MEAN_OFFSET,
    0.1513640 - MEAN_OFFSET
};


void main()
{     
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture(image, textCoord).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < SAMPLE_SIZE; ++i)
        {
            result += texture(image, textCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, textCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < SAMPLE_SIZE; ++i)
        {
            result += texture(image, textCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, textCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}