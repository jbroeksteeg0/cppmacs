#pragma once

#include <iostream>

const std::string test_vertex = "#version 330 core\n"
         "layout (location = 0) in vec3 aPos;\n"
         "void main()\n"
         "{\n"
         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
         "}\0";


const std::string test_fragment = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "void main()\n"
         "{\n"
         "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
         "}\n\0";

const std::string get_text_vertex = R"A(
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

const std::string get_text_fragment = R"A(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r); // set the alpha to the tex coord
    color = vec4(textColor, 1.0) * sampled; // textColor but with the alpha set by the letter
}
)A";

