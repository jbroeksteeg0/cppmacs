#include "FrameTree.h"

void __FRAMETREE_IMPL::Node::draw_all_frames() {
  if (type == __FRAMETREE_IMPL::Type::FRAME) {
    frame->draw();
  } else {
    left->draw_all_frames();
    right->draw_all_frames();
  }
}

void __FRAMETREE_IMPL::Node::update_frame_geometry(
  float x, float y, float width, float height
) {
  if (type == __FRAMETREE_IMPL::Type::FRAME) {
    frame->m_x = (int)x;
    frame->m_y = (int)y;
    frame->m_width = (int)width;
    frame->m_height = (int)height;

  } else if (type == __FRAMETREE_IMPL::Type::HSPLIT) {
    float left_width = width * split;
    left->update_frame_geometry(x, y, left_width, height);

    right->update_frame_geometry(x + left_width, y, width - left_width, height);
  } else if (type == __FRAMETREE_IMPL::Type::VSPLIT) {
    float top_height = height * split;
    left->update_frame_geometry(x, y, width, top_height);

    right->update_frame_geometry(x, y + top_height, width, height - top_height);
  }
}

void FrameTree::draw_all_frames() { m_root->draw_all_frames(); }

void FrameTree::update_frame_geometry(
  float x, float y, float width, float height
) {
  m_root->update_frame_geometry(x, y, width, height);
}

FrameTree::FrameTree(Window *parent, std::unique_ptr<Frame> root_frame)
    : m_window(parent) {
  m_root = std::make_shared<Node>(std::move(root_frame));
}

void FrameTree::create_frame_hsplit(Node *at, float split) {
  std::shared_ptr<Buffer> original_buffer = at->frame->m_buffer;
  at->type = HSPLIT;
  at->left = std::make_unique<Node>(std::move(at->frame));

  // Create a new frame using the previous buffer
  at->right =
    std::make_unique<Node>(std::make_unique<Frame>(original_buffer, m_window));
  at->split = split;
}

void FrameTree::create_frame_vsplit(Node *at, float split) {
  std::shared_ptr<Buffer> original_buffer = at->frame->m_buffer;

  at->type = VSPLIT;
  at->left = std::make_unique<Node>(std::move(at->frame));

  // Create a new frame using the previous buffer
  at->right =
    std::make_unique<Node>(std::make_unique<Frame>(original_buffer, m_window));
  at->split = split;
}
