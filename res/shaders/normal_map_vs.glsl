uniform mat4 u_MVPMatrix;
uniform mat4 u_MVMatrix;

attribute vec3 a_Position;
attribute vec2 a_TexCoord;

varying vec3 v_Position;
varying vec2 v_TexCoord;

void main() {
    v_Position = vec3(u_MVMatrix * vec4(a_Position, 1.0));
    v_TexCoord = a_TexCoord;
    gl_Position = u_MVPMatrix * vec4(a_Position, 1.0);
}
