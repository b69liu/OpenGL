#version 330 core

in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 TextCoord;

void main(){
    vec4  pos = projection * view * model * vec4(position,1.0);
   // vec4 pos = view * model * vec4(position,1.0);
	gl_Position = pos.xyww;
	TextCoord = position;
}
