#include "FrameTree.h"

FrameTree::FrameTree(std::unique_ptr<Frame> root_frame) {
  root = std::make_shared<Node>(std::move(root_frame));
}

void FrameTree::create_frame_hsplit(Node *at) {
  std::unique_ptr<Frame> prev_frame = std::move(at->frame);

  at->type = HSPLIT;
  at->left = std::make_shared<Node>(std::move(prev_frame));
  at->right = nullptr;
  at->split = 0.5;
}

void FrameTree::create_frame_vsplit(Node *at) {
  std::unique_ptr<Frame> prev_frame = std::move(at->frame);

  at->type = HSPLIT;
  at->left = std::make_shared<Node>(std::move(prev_frame));
  at->right = nullptr;
  at->split = 0.5;
}
