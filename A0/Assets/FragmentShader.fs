#version 330

in vec2 TextCoord;


uniform vec3 colour;
uniform sampler2D tex;
out vec4 fragColor;

void main() {
        fragColor = texture(tex,TextCoord) + vec4(colour,1.0) * 0.5 ;

}
