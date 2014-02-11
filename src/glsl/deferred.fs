#version 130

uniform sampler2D t_color;
uniform sampler2D t_normal;
uniform sampler2D t_depth;
uniform sampler2D t_light;

smooth in vec2 fuv;

out vec4 outColor;

// gbuffer samples
vec4 normal;
vec4 color;

vec4 applyLighting(vec4 colorVal)
{
    float SPECULARITY = 0.5;
    float AMBIENT = 0.25;

    float ambient;
    float difuse;
    float specular;
    float total;

    // SHOULD BE PASSED IN AS UNIFORM (mvp * lightpos)
    vec3 lightPos = normalize(vec3(100, 100, 0)); //TODO: LIGHT SHOULD BE VIEW SPACE
    vec3 h = normalize(lightPos + vec3(0,0,1));

    ambient = AMBIENT;

    difuse = clamp(dot(lightPos, normal.xyz), 0.0f, 1.0f);
    specular = clamp(SPECULARITY * pow(dot(normal.xyz, h), 32.0f), 0.0f, 1.0f) 
        / length(texture(t_depth, fuv));
    
    total = max(ambient, difuse);

    return vec4(total * colorVal.rgb + vec3(specular), 1);
}

void main()
{
    color = texture(t_color, fuv);
    normal = texture(t_normal, fuv);
    color = applyLighting(color);
    outColor = texture(t_light, fuv); //color;
}
