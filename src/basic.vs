#version 410 core

layout (location = 0) in vec2 aPos;
out vec4 vertexColor;

uniform vec2 scale = vec2(1.0, 1.0);
uniform float rotation = 0.0;
uniform vec2 translation = vec2(0.0, 0.0);
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

void main() {
  mat2 R = mat2(cos(rotation), -sin(rotation),
                sin(rotation), cos(rotation));
  gl_Position = vec4((R * (scale * aPos)) + translation, 0.0, 1.0);
  vertexColor = color;
}
