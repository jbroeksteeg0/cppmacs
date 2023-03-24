#pragma once

#include <iostream>

const std::string text_vertex_shader = R"A(
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

const std::string text_fragment_shader = R"A(
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

const std::string standard_vertex_shader = R"A(
#version 330 core
in vec2 aPos;

uniform mat4 projection;

void main()
{
   gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
}
)A";

const std::string standard_fragment_shader = R"A(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
} 
)A";
