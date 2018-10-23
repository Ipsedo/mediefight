precision mediump float;
uniform sampler2D u_tex;
uniform sampler2D u_normalMap;

uniform vec3 u_LightPos;
uniform float u_distance_coef;
uniform float u_light_coef;

varying vec3 v_Position;
varying vec2 v_TexCoord;
varying mat3 v_TBN;

void main() {
    vec3 normalColor = texture2D(u_normalMap, v_TexCoord).rgb;
    vec3 texColor = texture2D(u_tex, v_TexCoord).rgb;

    vec3 normal = normalize(v_TBN * normalize(2.0 * normalColor - 1.0));

    float distance = length(u_LightPos - v_Position);

    vec3 lightVector = normalize(v_TBN * normalize(u_LightPos - v_Position));

    float diffuse = max(dot(normal, lightVector), 0.1) * u_light_coef;

    diffuse = diffuse * (1.0 / (1.0 + (u_distance_coef * distance * distance)));

    gl_FragColor = vec4(texColor, 1.0) * diffuse;
}
