#include "FrameTree.h"

void __FRAMETREE_IMPL::Node::draw_all_frames(FrameTree *tree
) {
  if (type == __FRAMETREE_IMPL::Type::FRAME) {
    frame->draw();
  } else {
    left->draw_all_frames(tree);
    right->draw_all_frames(tree);
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

    right->update_frame_geometry(
      x + left_width, y, width - left_width, height
    );
  } else if (type == __FRAMETREE_IMPL::Type::VSPLIT) { // TODO: this is still clapped
    float top_height = height * split;
    left->update_frame_geometry(
      x, height - top_height, width, top_height
    );

    right->update_frame_geometry(
      x, y, width, height - top_height
    );
  }
}

void FrameTree::draw_all_frames() {
  m_root->draw_all_frames(this);
}

void FrameTree::update_frame_geometry(
  float x, float y, float width, float height
) {
  m_root->update_frame_geometry(x, y, width, height);
}

FrameTree::FrameTree(
  Window *parent, std::unique_ptr<Frame> root_frame
) {
  m_root = std::make_shared<Node>(std::move(root_frame));
  m_window = parent;
  m_selected = m_root.get();
}

void FrameTree::create_frame_hsplit(Node *at, float split) {
  std::shared_ptr<Buffer> original_buffer =
    at->frame->m_buffer;
  // Stores whether the current selected node has to change
  bool reset_selected = at == m_selected;

  at->type = HSPLIT;
  at->left = std::make_unique<Node>(std::move(at->frame));

  // Create a new frame using the previous buffer
  at->right = std::make_unique<Node>(
    std::make_unique<Frame>(original_buffer, m_window)
  );
  at->split = split;
  if (reset_selected) {
    m_selected = at->left.get();
  }
}

void FrameTree::create_frame_vsplit(Node *at, float split) {
  std::shared_ptr<Buffer> original_buffer =
    at->frame->m_buffer;
  bool reset_selected = at == m_selected;

  at->type = VSPLIT;
  at->left = std::make_unique<Node>(std::move(at->frame));

  // Create a new frame using the previous buffers
  at->right = std::make_unique<Node>(
    std::make_unique<Frame>(original_buffer, m_window)
  );
  at->split = split;

  if (reset_selected) {
    m_selected = at->left.get();
    std::cout << "Selected is " << m_selected << std::endl;
  }
}
