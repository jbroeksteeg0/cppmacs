#pragma once

#include <iostream>

constexpr std::string test_vertex() {
  return "#version 330 core\n"
         "layout (location = 0) in vec3 aPos;\n"
         "void main()\n"
         "{\n"
         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
         "}\0";
}

constexpr std::string test_fragment() {
  return "#version 330 core\n"
         "out vec4 FragColor;\n"
         "void main()\n"
         "{\n"
         "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
         "}\n\0";
}

constexpr std::string get_text_vertex() {
  return R"A(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)A";
}

constexpr std::string get_text_fragment() {
  return R"A(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    //color = vec4(textColor, 1.0) * sampled;
    color = vec4(textColor, 1.0);
}
)A";
}
