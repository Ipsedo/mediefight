precision mediump float;
uniform mat4 u_MVMatrix;
uniform sampler2D u_tex;
uniform sampler2D u_normalMap;

uniform vec3 u_LightPos;
uniform float u_distance_coef;
uniform float u_light_coef;

varying vec3 v_Position;
varying vec2 v_TexCoord;

vec3 toNormal(vec3 angles) {
    vec3 res = normalize(2.0 * angles - 1.0);
    return res;
}

void main() {
    vec3 normalColor = texture2D(u_normalMap, v_TexCoord).rgb;
    vec3 colors = texture2D(u_tex, v_TexCoord).rgb;

    vec3 normal = toNormal(normalColor);

    float distance = length(u_LightPos - v_Position);

    vec3 lightVector = normalize(u_LightPos - v_Position);

    float diffuse = max(dot(normal, lightVector), 0.1) * u_light_coef;

    diffuse = diffuse * (1.0 / (1.0 + (u_distance_coef * distance * distance)));
    gl_FragColor = vec4(colors, 1.0) * diffuse;
}
