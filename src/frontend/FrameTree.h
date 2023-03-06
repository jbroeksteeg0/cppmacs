#pragma once
#include "Frame.h"
#include <memory>

class FrameTree;
class Frame;

namespace __FRAMETREE_IMPL {

enum class Type { FRAME, HSPLIT, VSPLIT };
using enum Type;

struct Node {
  std::unique_ptr<Node> left, right;
  Type type;
  std::unique_ptr<Frame> frame;
  float split;

  int x, y, width, height;
  // leaf
  Node(std::unique_ptr<Frame> frame)
      : left(nullptr), right(nullptr), type(FRAME),
        frame(std::move(frame)), split(1.0f) {}

  // non leaf
  Node(
    Type type,
    float split,
    std::unique_ptr<Node> left,
    std::unique_ptr<Node> right
  )
      : left(std::move(left)), right(std::move(right)),
        type(type), frame(nullptr), split(split) {}

  friend class FrameTree;

  void draw_and_recurse();
  void update_dimensions(int x, int y, int width, int height);
};

};    // namespace __FRAMETREE_IMPL

class FrameTree {
  using Node = __FRAMETREE_IMPL::Node;
  using enum __FRAMETREE_IMPL::Type;

public:
  FrameTree(Window *parent);
  FrameTree(
    Window *parent, std::unique_ptr<Frame> root_frame
  );

  void create_frame_hsplit(Node *at);
  void create_frame_vsplit(Node *at);
  void delete_frame(Node *to_delete);

  void draw_all(int window_width, int window_height);

private:
  Window *m_window;
  std::shared_ptr<Node> m_root;
};
