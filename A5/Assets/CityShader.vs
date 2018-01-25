#version 330

in vec3 position;
in vec2 textCoord;

//uniform mat4 transform;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec2 TextCoord;

void main() {
        //gl_Position = transform * vec4(position, 1.0);
    gl_Position = P * V * M * vec4(position, 1.0);
	TextCoord = textCoord;
}
