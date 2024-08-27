varying vec3 lightDir;

varying vec3 normal;
varying vec3 binormal;
varying vec3 tangent;

varying vec4 material_diffuse;
varying vec4 material_ambient;

varying vec2 tex0, tex1;

uniform sampler2D NormalMap;
uniform sampler2D DiffuseMap;

void main() {
    vec4 diffuse_tex = texture2D(DiffuseMap, tex1);
    vec3 normal_tex = texture2D(NormalMap, tex0).rgb * 2.0 - 1.0;

    normal_tex = normalize(normal_tex.x * tangent +
                           normal_tex.y * binormal +
                           normal_tex.z * normal);

    float intensity = clamp(dot(normal_tex, lightDir), 0.0, 1.0);
    vec4 color = material_diffuse.rgba * diffuse_tex * intensity;
    color += material_ambient.rgba * diffuse_tex;
    gl_FragColor = color;
}

