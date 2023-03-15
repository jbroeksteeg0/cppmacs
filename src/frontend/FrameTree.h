#pragma once
#include "Frame.h"
#include <memory>

struct Frame;

namespace __FRAMETREE_IMPL {

enum class Type { FRAME, HSPLIT, VSPLIT };
using enum Type;

struct Node {
  std::shared_ptr<Node> left, right;
  Type type;
  std::unique_ptr<Frame> frame;
  float split;

  // leaf
  Node(std::unique_ptr<Frame> frame)
      : left(nullptr), right(nullptr), type(FRAME), frame(std::move(frame)),
        split(1.0f) {}

  // non leaf
  Node(
    Type type,
    float split,
    std::shared_ptr<Node> left,
    std::shared_ptr<Node> right
  )
      : left(left), right(right), type(type), frame(nullptr), split(split) {}

  void draw_all_frames();
  void update_frame_geometry(float x, float y, float width, float height);

  friend class FrameTree;
};

};    // namespace __FRAMETREE_IMPL

class FrameTree {
  using Node = __FRAMETREE_IMPL::Node;
  using enum __FRAMETREE_IMPL::Type;
public:
  FrameTree(std::unique_ptr<Frame> root_frame);

  void create_frame_hsplit(Node *at, float split);
  void create_frame_vsplit(Node *at, float split);
  void delete_frame(Node *to_delete);

  void draw_all_frames();
  void update_frame_geometry(float x, float y, float width, float height);
  std::shared_ptr<Node> m_root;
private:
};
