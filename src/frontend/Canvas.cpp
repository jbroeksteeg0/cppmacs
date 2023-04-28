#include "Canvas.h"
#include "../misc/Logger.h"
#include "Shaders.h"
#include "Window.h"
#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/chrono.h>
#include <ft2build.h>
#include <ratio>

#include FT_FREETYPE_H

Canvas::Canvas(Window *ptr) : m_ptr(ptr) {
  m_text_program =
    Program(text_vertex_shader, text_fragment_shader);

  m_geometry_program = Program(
    standard_vertex_shader, standard_fragment_shader
  );

  init_text();
  init_geometry_buffers();
}

void Canvas::init_text() {
  FT_Library ft;
  FT_Init_FreeType(&ft);
  FT_Face face;
  if (FT_New_Face(
        ft,
        "/usr/share/fonts/TTF/FiraCode-Medium.ttf",
        0,
        &face
      )) {
    ERROR("Error initialising face");
    exit(1);
  }

  // set height to 48, width to natural
  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(
    GL_UNPACK_ALIGNMENT, 1
  );    // something to do with alignment

  // load the chars
  for (int i = 0; i < 128; i++) {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
      ERROR("Failed to load char");
      exit(1);
    }

    // make the texture in opengl
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );

    // set texture options in opengl
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
    );
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
    );
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
    );
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
    );

    m_characters.push_back(
      {texture,
       glm::ivec2(
         face->glyph->bitmap.width, face->glyph->bitmap.rows
       ),
       glm::ivec2(
         face->glyph->bitmap_left, face->glyph->bitmap_top
       ),
       face->glyph->advance.x}
    );
  }
  // free resources
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenVertexArrays(1, &m_text_VAO);
  glGenBuffers(1, &m_text_VBO);
  glBindVertexArray(m_text_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_text_VBO);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(float) * 6 * 4,
    NULL,
    GL_DYNAMIC_DRAW
  );
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0
  );
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Canvas::draw_text(
  std::string text,
  float x,
  float y,
  std::array<float, 3> color
) {
  float scale = 1.0;

  // activate corresponding render state
  m_text_program.use();

  // set the color of the text
  glUniform3f(
    glGetUniformLocation(
      m_text_program.get_program(), "textColor"
    ),
    color[0],
    color[1],
    color[2]
  );

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(m_text_VAO);

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = m_characters[*c];

    float x_pos = x + ch.bearing.x * scale;
    float y_pos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
    // update m_text_VBO for each character
    float vertices[6][4] = {
      {x_pos, y_pos + h, 0.0f, 0.0f},
      {x_pos, y_pos, 0.0f, 1.0f},
      {x_pos + w, y_pos, 1.0f, 1.0f},

      {x_pos, y_pos + h, 0.0f, 0.0f},
      {x_pos + w, y_pos, 1.0f, 1.0f},
      {x_pos + w, y_pos + h, 1.0f, 0.0f}};

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.id);

    // update content of m_text_VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, m_text_VBO);
    glBufferSubData(
      GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices
    );    // glBufferSubdata is faster

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // now advance cursors for next glyph (note that advance
    // is number of 1/64 pixels)
    x +=
      (ch.advance >> 6)
      * scale;    // bitshift by 6 to get value in pixels
                  // (2^6 = 64 (divide amount of 1/64th
                  // pixels by 64 to get amount of pixels))
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Canvas::set_ortho_projection(
  float width, float height
) {
  glm::mat4 projection =
    glm::ortho(0.0f, width, 0.0f, height);

  m_text_program.use();
  glUniformMatrix4fv(
    glGetUniformLocation(
      m_text_program.get_program(), "projection"
    ),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );

  m_geometry_program.use();
  glUniformMatrix4fv(
    glGetUniformLocation(
      m_geometry_program.get_program(), "projection"
    ),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );
}

void Canvas::init_geometry_buffers() {
  glGenBuffers(1, &this->m_rect_VAO);
  glGenBuffers(1, &this->m_rect_VBO);
}

void Canvas::draw_rectangle(
  float x, float y, float width, float height
) {
  float vertices[12] = {
    x,
    y,
    x + width,
    y,
    x + width,
    y + height,

    x + width,
    y + height,
    x,
    y + height,
    x,
    y};

  glBindBuffer(GL_ARRAY_BUFFER, this->m_rect_VBO);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(vertices),
    vertices,
    GL_STATIC_DRAW
  );

  glBindVertexArray(this->m_rect_VAO);
  glVertexAttribPointer(
    0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0
  );
  m_geometry_program.use();
  glEnableVertexAttribArray(0);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Canvas::text_box_init(
  int x, int y, int width, int height
) {
  m_text_box_x = x;
  m_text_box_y = y;

  m_text_box_width = width;
  m_text_box_height = height;

  // offset = height - max(height of characters)
  std::string all_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  m_text_box_offset_y =
    height - get_text_dimensions(all_chars).second
    - m_line_spacing;
}

void Canvas::text_box_write_line(
  const std::string &text, int cursor_pos
) {
  std::string prefix;
  prefix.reserve(text.size());

  std::vector<std::string> lines;

  for (size_t i = 0; i < text.size(); i++) {
    ASSERT(
      text[i] != '\n',
      "Newlines should be handled in the rope"
    );

    prefix += text[i];

    // if needs to wrap
    if (get_text_dimensions(prefix).first + 6 > m_text_box_width) {
      prefix.pop_back();
      lines.push_back(prefix);
      prefix = std::string(1, text[i]);
    }

    if (i + 1 == text.size()) {    // done
      lines.push_back(prefix);
    }
  }

  // Handle newlines
  if (lines.empty()) {
    lines = {""};
  }

  std::string all_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  for (std::string line : lines) {
    draw_text(line, m_text_box_x, m_text_box_offset_y);
    if (cursor_pos >= 0 && cursor_pos <= (int)line.size()) {
      std::string prefix = line.substr(0, cursor_pos);

      using namespace std::chrono;

      milliseconds time_since_typed =
        duration_cast<milliseconds>(
          system_clock::now().time_since_epoch()
          - m_time_text_last_modified
        );

      // whether the cursor should be hidden at the current
      // moment
      bool cursor_flashing =
        m_cursor_flash_duration > 0
        && (time_since_typed.count() % m_cursor_flash_duration < m_cursor_flash_duration / 2)
        && time_since_typed.count() > 500;

      if (!cursor_flashing) {
        draw_rectangle(
          get_text_dimensions(prefix).first,
          m_text_box_offset_y-2,
          m_cursor_width,
          get_text_dimensions(all_chars).second+4
        );
      }

    } else {
      cursor_pos -= line.size();
    }

    m_text_box_offset_y -=
      get_text_dimensions(all_chars).second
      + m_line_spacing;
  }
}

bool Canvas::text_box_is_finished() const {
  return m_text_box_offset_y > m_text_box_height;
}

std::pair<int, int>
Canvas::get_text_dimensions(const std::string &text) const {
  int width = 0, height = 0;

  for (size_t i = 0; i < text.size(); i++) {
    Character ch = m_characters[(int)text[i]];

    if (text[i] == ' ') {
      width += m_characters['A'].advance >> 6;
    } else if (i + 1 == text.size()) {
      width += ch.bearing.x + ch.size.x;
    } else {
      width += ch.advance >> 6;
    }

    height = std::max((int)height, (int)ch.bearing.y);
  }
  return {width, height};
}

void Canvas::text_box_key_pressed() {
  m_time_text_last_modified =
    std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
    );
}
