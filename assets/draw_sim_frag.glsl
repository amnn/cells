#version 330 core

uniform usampler2DArray publicSimSampler;
uniform float           time;

out vec4 color;

uniform vec3[8] teamColors = vec3[8](
    vec3(1.f, 0.f, 0.f),
    vec3(0.f, 1.f, 0.f),
    vec3(0.f, 0.f, 1.f),
    vec3(1.f, 0.f, 1.f),
    vec3(1.f, 1.f, 0.f),
    vec3(0.f, 1.f, 1.f),
    vec3(1.f, 1.f, 1.f),
    vec3(0.f, 0.f, 0.f)
);

#define TERRAIN_SHADE vec4(0.75, 0.56, 0.34, 1.0)
#define ENERGY_SHADE  vec3(0.16, 0.56, 0.99)

vec4 fetchLayer(int);
bool renderAgents( );
void renderTerrain();

void
main()
{
    if(!renderAgents()) renderTerrain();
}

vec4 fetchLayer(int layer)
{
    return texelFetch(publicSimSampler, ivec3(gl_FragCoord.xy, layer), 0);
}

bool
renderAgents()
{
    int layers = textureSize(publicSimSampler, 0).z;

    vec4 texel;
    for(int i = layers-1; i > 0; --i)
    {
        texel = fetchLayer(i);
        if(texel.r > 0)
        {
            color.rgb = teamColors[i-1];
            return true;
        }
    }

    return false;
}

void
renderTerrain()
{
    vec4 texel = fetchLayer(0);
    color      = ((texel.r + 5u)/10.0) * TERRAIN_SHADE;

    if(texel.g > 0)
    {
        float offset = uint(gl_FragCoord.x + gl_FragCoord.y) % 10u / 10.f;
        float glow   = abs( fract(time/2.f + offset) - .5f ) * 2;
        float amp    = glow * texel.g / 10.f;

        color.rgb    = mix(color.rgb, ENERGY_SHADE, amp);
    }
}
