#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 ProjectionView;
    vec4 AmbientLightColor; // w = intensity
    vec3 LightPosition;
    vec4 LightColor; // w - is light intensity
} ubo;


layout(push_constant) uniform Push {
    mat4 Transform; // proj * view * model
    mat4 ModelMatrix;
    vec3 LightTransform;
} push;

void main(){
    vec3 newPos = vec3(fragPosWorld.x, fragPosWorld.y, fragPosWorld.z);
    vec3 dir_to_light = ubo.LightPosition;

    vec3 DirectionToLight = dir_to_light - newPos.xyz;
    float attenuation = 1.0 / dot(DirectionToLight, DirectionToLight);

    vec3 LightColor = ubo.LightColor.xyz * ubo.LightColor.w;
    vec3 AmbientLight = ubo.AmbientLightColor.xyz * ubo.AmbientLightColor.w * attenuation;
    vec3 DiffuseLighting = LightColor * max(dot(fragNormalWorld, normalize(DirectionToLight)), 0);

    outColor = vec4((DiffuseLighting + AmbientLight) * fragColor, 1.0);

}