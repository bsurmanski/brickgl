#version 130

uniform sampler2D t_normal;
uniform sampler2D t_position;
uniform sampler2D t_depth;
// uniform sampler2D t_shadow; // shadow buffer?

uniform vec4 light; // screen space light position
// uniform vec4 ads; // TODO ambient defuse specular properties
// uniform vec4 color; // TODO light color

smooth in vec2 fuv;

out vec4 lightAccum;

// gbuffer samples
vec4 normal;
vec4 position;
vec4 depth;

vec4 applyLighting()
{
    float SPECULARITY = 0.5;
    float AMBIENT = 0.25;

    float ambient;
    float difuse;
    float specular;
    float total;

    vec3 pixelPos = position.xyz;
    vec3 lightDir = normalize(light.xyz - pixelPos);
    vec3 h = (lightDir + vec3(0,0,1)) / length(lightDir + vec3(0,0,1));

    ambient = AMBIENT;

    difuse = clamp(dot(lightDir, normal.xyz), 0.0f, 1.0f);
    specular = clamp(SPECULARITY * pow(dot(normal.xyz, h), 32.0f), 0.0f, 1.0f) 
        / length(depth);
    
    total = max(ambient, difuse);

    return vec4(difuse + vec3(specular), length(depth));
}

void main()
{
    normal = texture(t_normal, fuv);
    depth = texture(t_depth, fuv);
    position = texture(t_position, fuv);
    lightAccum = applyLighting();
}
