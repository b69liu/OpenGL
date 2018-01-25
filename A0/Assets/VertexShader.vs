#version 330

in vec3 position;
in vec2 textCoord;

uniform mat4 transform;

out vec2 TextCoord;

void main() {
	gl_Position = transform * vec4(position, 1.0);
        TextCoord = textCoord;
}
