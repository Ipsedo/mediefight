#version 150
uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec3 a_Tangent;
attribute vec3 a_BiTangent;
attribute vec2 a_TexCoord;

varying vec3 v_Position;
varying vec2 v_TexCoord;
varying mat3 v_TBN;

void main() {
    v_Position = vec3(u_MVMatrix * vec4(a_Position, 1.0));

    v_TexCoord = a_TexCoord;

    vec3 T = normalize(vec3(u_MVMatrix * vec4(a_Tangent,   0.0)));
    vec3 B = normalize(vec3(u_MVMatrix * vec4(a_BiTangent, 0.0)));
    vec3 N = normalize(vec3(u_MVMatrix * vec4(a_Normal,    0.0)));
    v_TBN = transpose(mat3(T, B, N));

    gl_Position = u_MVPMatrix * vec4(a_Position, 1.0);
}
