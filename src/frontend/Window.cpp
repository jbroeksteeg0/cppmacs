#include "Window.h"
#include "../misc/Logger.h"
#include <ft2build.h>

#include FT_FREETYPE_H

Window::Window() {
  glfwInit();

  m_window = glfwCreateWindow(800, 600, "window", nullptr, nullptr);
  glfwMakeContextCurrent(m_window);

  if (glewInit() != GLEW_OK) {
    ERROR("Error initialising glew");
    printf("%s\n", glewGetErrorString(glewInit()));
    exit(1);
  }
  init_freetype();

  m_text_program = Program(get_text_vertex(), get_text_fragment());
  init_text();
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void test_draw() {
  float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f
  };

  Program test_program = Program(get_text_vertex(), get_text_fragment());
  test_program.use();

  glViewport(0, 0, 800, 600);

  glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

  glUniformMatrix4fv(
    glGetUniformLocation(test_program.get_program(), "projection"),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );

  glUniform3f(
    glGetUniformLocation(test_program.get_program(), "textColor"),
    1.0f,
    0.0f,
    0.0f
  );

  glUniform1i(glGetUniformLocation(test_program.get_program(), "text"), 0);

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  while (glGetError() != GL_NO_ERROR) {
    LOG("ERROR");
  }
}

void Window::run() {
  while (!glfwWindowShouldClose(m_window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    test_draw();
    
    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}

void Window::init_freetype() {
  FT_Library ft;
  FT_Init_FreeType(&ft);
  FT_Face face;
  if (FT_New_Face(ft, "/usr/share/fonts/TTF/FiraCode-Medium.ttf", 0, &face)) {
    ERROR("Error initialising face");
    exit(1);
  }

  // set height to 48, width to natural
  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);    // something to do with alignment

  // load the chars
  for (int i = 0; i < 128; i++) {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
      ERROR("Failed to load char");
      exit(1);
    }
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  m_characters.push_back(
    {texture,
     glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
     glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
     face->glyph->advance.x}
  );
  // free resources
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::init_text() {
  glGenVertexArrays(1, &m_text_VAO);
  glGenBuffers(1, &m_text_VBO);
  glBindVertexArray(m_text_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_text_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Window::draw_text(std::string text) {
  float scale = 1.0;
  // activate corresponding render state
  glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

  m_text_program.use();
  glUniformMatrix4fv(
    glGetUniformLocation(m_text_program.get_program(), "projection"),
    1,
    GL_FALSE,
    glm::value_ptr(projection)
  );

  glUniform3f(
    glGetUniformLocation(m_text_program.get_program(), "textColor"),
    1.0f,
    0.0f,
    0.0f
  );

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(m_text_VAO);

  float x = 50.0f, y = 50.0f;

  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = m_characters[*c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
    // update m_text_VBO for each character
    float vertices[6][4] = {
      {xpos, ypos + h, 0.0f, 0.0f},
      {xpos, ypos, 0.0f, 1.0f},
      {xpos + w, ypos, 1.0f, 1.0f},

      {xpos, ypos + h, 0.0f, 0.0f},
      {xpos + w, ypos, 1.0f, 1.0f},
      {xpos + w, ypos + h, 1.0f, 0.0f}};
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.id);
    // update content of m_text_VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, m_text_VBO);
    glBufferSubData(
      GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices
    );    // be sure to use glBufferSubData and not glBufferData

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64
    // pixels)
    x += (ch.advance >> 6)
         * scale;    // bitshift by 6 to get value in pixels (2^6 = 64 (divide
                     // amount of 1/64th pixels by 64 to get amount of pixels))
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
