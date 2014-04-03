#version 130

uniform sampler2D t_normal;
uniform sampler2D t_position;
uniform sampler2D t_depth;
// uniform sampler2D t_shadow; // shadow buffer?

uniform vec4 camera;  // world space camera position
uniform vec4 light; // world space light position
uniform vec4 color;
// uniform vec4 ads; // TODO ambient defuse specular properties
// uniform vec4 color; // TODO light color

smooth in vec2 fuv;

out vec4 lightAccum;

// gbuffer samples
vec4 normal;
vec4 position;
vec4 depth;

float DIST_SCALE = 100.0f;

vec4 applyLighting()
{
    float SPECULARITY = 0.5;
    float SPEC_POWER = 20.0f;

    float difuse;
    float specular;
    float total;

    vec3 lightDir = normalize(light.xyz - position.xyz); 
    vec3 cameraDir = normalize(camera.xyz - position.xyz);

    // used in specular only
    vec3 h = normalize(lightDir + cameraDir);

    difuse = clamp(
        dot(lightDir, normal.xyz), 
        0.0f, 
        1.0f
    );

    specular = clamp(SPECULARITY * pow(dot(normal.xyz, h), 32.0f), 0.0f, 1.0f) * 
        (SPEC_POWER / (length(light.xyz - position.xyz) * DIST_SCALE)); 
    
    return vec4((difuse * color).xyz, specular); 
}

void main()
{
    normal = normalize(texture(t_normal, fuv));
    depth = texture(t_depth, fuv);
    position = texture(t_position, fuv);

    lightAccum = applyLighting();
}
