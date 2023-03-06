#include "FrameTree.h"

FrameTree::FrameTree(Window *parent) : m_window(parent) {
  m_root = std::make_shared<Node>(std::make_unique<Frame>(
    std::make_shared<Buffer>(), parent
  ));
}

FrameTree::FrameTree(
  Window *parent, std::unique_ptr<Frame> root_frame
)
    : m_window(parent) {
  m_root = std::make_shared<Node>(std::move(root_frame));
}

void FrameTree::create_frame_hsplit(Node *at) {
  std::shared_ptr<Buffer> original_buffer =
    at->frame->m_buffer;

  at->type = HSPLIT;
  at->left = std::make_unique<Node>(std::move(at->frame));

  // Create a new frame using the previous buffer
  at->right = std::make_unique<Node>(
    std::make_unique<Frame>(m_window, original_buffer)
  );
  at->split = 0.5;
}

void FrameTree::create_frame_vsplit(Node *at) {
  Window *window = at->frame->m_window;
  std::shared_ptr<Buffer> original_buffer =
    at->frame->m_buffer;

  at->type = VSPLIT;
  at->left = std::make_unique<Node>(std::move(at->frame));

  // Create a new frame using the previous buffer
  at->right = std::make_unique<Node>(
    std::make_unique<Frame>(m_window, original_buffer)
  );
  at->split = 0.5;
}

void FrameTree::draw_all(
  int window_width, int window_height
) {
  m_root->update_dimensions(
    0, 0, window_width, window_height
  );
  m_root->draw_and_recurse();
}

void __FRAMETREE_IMPL::Node::draw_and_recurse() {
  if (type == FRAME) {
    glViewport(x, y, width, height);
    frame->draw();
  } else {
    left->draw_and_recurse();
    right->draw_and_recurse();
  }
}

void __FRAMETREE_IMPL::Node::update_dimensions(
  int curr_x, int curr_y, int curr_width, int curr_height
) {
  x = curr_x;
  y = curr_y;
  width = curr_width;
  height = curr_height;

  int top_height = float(height) * split;

  // TODO check for off by one's
  if (type == VSPLIT) {
    left->update_dimensions(
      x, y + top_height, width, top_height
    );

    right->update_dimensions(
      x, y, width, height - top_height
    );
  } else if (type == HSPLIT) {

  }
}
