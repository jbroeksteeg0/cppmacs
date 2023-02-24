#include "Window.h"
#include "../misc/Logger.h"
#include <ft2build.h>
#include FT_FREETYPE_H

Window::Window() {
  glfwInit();
  init_freetype();

  window = glfwCreateWindow(640, 480, "window", nullptr, nullptr);
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::run() {
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
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
}
