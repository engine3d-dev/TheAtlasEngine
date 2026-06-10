#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoords;
layout(location = 2) in vec3 fragNormals;
layout(location = 3) in flat int fragDiffuseIdx;
// layout(location = 4) in flat int fragSpecularIdx;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D textures[];
layout(set = 0, binding = 2) uniform sampler2D environment_map; // used for IBL

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleEquirectangular(vec3 v) {
    vec3 direction = normalize(v);
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec3 color = fragColor;
    vec4 diffuse = texture(textures[nonuniformEXT(fragDiffuseIdx)], fragTexCoords);
    // vec4 specular = texture(textures[nonuniformEXT(fragSpecularIdx)], fragTexCoords);

    vec3 albedo = diffuse.rgb;
    // Map the surface normal direction to the HDR map to get diffuse ambient light
    vec3 N = normalize(fragNormals);
    vec2 envUV = SampleEquirectangular(N);
    // Low-frequency ambient lighting from the surrounding environment
    // vec3 ambientIrradiance = texture(environment_map, envUV).rgb;
    vec3 ambientIrradiance = textureLod(environment_map, envUV, 4.0).rgb;

    // In a full PBR equation, this would be multiplied by your diffuse term (1.0 - metallic) * albedo
    vec3 ambient = ambientIrradiance * albedo * 0.05; // Dim down slightly for a basic placeholder balance

    // 4. Combine lighting components (Direct light + Ambient IBL)
    vec3 final_color = albedo + ambient * color;

    outColor = vec4(final_color, 1.0);
}