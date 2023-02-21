#pragma once

#include "../misc/Tests.h"
#include "../misc/Util.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>

template <typename T> class Rope;

namespace __ROPE_IMPL {

template <typename T> struct AVLNode {
  std::shared_ptr<AVLNode> left, right;
  AVLNode *parent, *prev, *succ;
  T value;
  int depth = 1, size = 1;

  AVLNode() = delete;
  AVLNode(T value, AVLNode *parent, AVLNode *prev, AVLNode *succ)
      : parent(parent), prev(prev), succ(succ), value(value) {}

  AVLNode(
    T value,
    std::shared_ptr<AVLNode> left,
    std::shared_ptr<AVLNode> right,
    AVLNode *parent,
    AVLNode *prev,
    AVLNode *succ
  )
      : left(left), right(right), parent(parent), prev(prev), succ(succ),
        value(value) {}

  friend class Rope;
};

};    // namespace __ROPE_IMPL

template <typename T> class Rope {
  using Node = __ROPE_IMPL::AVLNode<T>;

public:
  Rope() {}

  void insert(T value, int position) {
    ASSERT(position <= m_size, "Position must be <= size");
    if (m_root == nullptr) {
      ASSERT(position == 0, "Position must be 0 if rope is empty");
      m_root = std::make_shared<Node>(value, nullptr, nullptr, nullptr);
    } else {
      // find predecessor and success pointers
      Node *prev =
             position != 0 ? find_node_pointer(&m_root, position - 1) : nullptr,
           *succ = position < (int)size() ? find_node_pointer(&m_root, position)
                                          : nullptr;

      internal_insert(m_root, value, position, prev, succ);

      rotate_if_necessary(&m_root);
    }

    m_size++;
  }

  void erase(int ind) {
    ASSERT(ind < m_size, "Index for erasing must be < size");

    internal_erase(&m_root, ind);

    m_size--;
    if (m_size == 0)
      m_root = nullptr;
  }

  T at(int ind) const {
    ASSERT(ind < m_size, "Index for accessing must be < size");
    return internal_at(m_root, ind);
  }

  std::string to_string() const {
    if (m_root == nullptr)
      return "[]";

    std::stringstream ss;
    ss << "[";
    ss << intern_to_string(*m_root).str();
    ss << "]";
    return ss.str();
  }

  int depth() const {
    if (m_root == nullptr)
      return 0;
    return m_root->depth;
  }

  size_t size() const { return (size_t)m_size; }

  struct Iterator {
    Iterator &operator++() {
      ASSERT(current != nullptr, "Cannot increment nullptr");
      current = current->succ;
      return *this;
    }
    Iterator &operator--() {
      ASSERT(current != nullptr, "Cannot decrement nullptr");
      current = current->prev;
      return *this;
    }

    T operator*() {
      ASSERT(current != nullptr, "Cannot dereference nullptr");
      return current->value;
    }

    bool operator==(const Iterator &rhs) const {
      return rhs.current == current;
    }
    bool operator!=(const Iterator &rhs) const {
      return rhs.current != current;
    }

  private:
    Iterator(Node *node) : current(node) {}
    Node *current;

    friend class Rope;
  };

  Iterator begin() const {
    if (m_root == nullptr)
      return nullptr;
    return find_node_pointer(&m_root, 0);
  }
  Iterator end() const { return Iterator(nullptr); }

private:
  std::shared_ptr<Node> m_root = nullptr;
  int m_size = 0;

  bool needs_rebalance(Node &node) const { return abs(get_balance(node)) > 1; }

  int get_balance(Node &node) const {
    int on_left = node.left != nullptr ? node.left->depth : 0;
    int on_right = node.right != nullptr ? node.right->depth : 0;
    return on_right - on_left;
  }

  std::stringstream intern_to_string(Node &node) const {
    std::stringstream ss;
    if (node.left != nullptr) {
      ss << intern_to_string(*node.left).str() << ", ";
    }
    ss << node.value;
    if (node.right != nullptr) {
      ss << ", " << intern_to_string(*node.right).str();
    }
    return ss;
  }

  void update_depth_and_size(Node &node) {
    node.depth = 1
                 + std::max(
                   node.left == nullptr ? 0 : node.left->depth,
                   node.right == nullptr ? 0 : node.right->depth
                 );
    node.size = 1 + (node.left == nullptr ? 0 : node.left->size)
                + (node.right == nullptr ? 0 : node.right->size);
  }

  // Pass in parent's ptr
  Node *find_node_pointer(const std::shared_ptr<Node> *node, int index) const {
    ASSERT(index < m_size, "find index cannot be out of bounds");
    std::shared_ptr<Node> curr = *node;
    int on_left = curr->left == nullptr ? 0 : curr->left->size;
    if (index < on_left) {
      return find_node_pointer(&curr->left, index);    // recurse on left
    } else if (index == on_left) {
      return curr.get();    // return current
    } else {
      return find_node_pointer(
        &curr->right, index - on_left - 1
      );    // recurse on right
    }
  }
  // pass in the parent's ptr to the node
  void rotate_left(std::shared_ptr<Node> *parents_ptr) {
    ASSERT((*parents_ptr)->right != nullptr, "right ptr cannot be null");

    Node &oldX = **parents_ptr;

    std::shared_ptr<Node> newX = std::make_shared<Node>(
      oldX.value,
      oldX.left,
      oldX.right->left,
      nullptr,    // equal to newZ
      oldX.prev,
      oldX.succ
    );

    if (oldX.prev != nullptr)
      oldX.prev->succ = newX.get();
    if (oldX.succ != nullptr)
      oldX.succ->prev = newX.get();

    std::shared_ptr<Node> newZ = std::make_shared<Node>(
      oldX.right->value,
      newX,
      oldX.right->right,
      oldX.parent,
      oldX.right->prev,
      oldX.right->succ
    );

    if (oldX.right->prev != nullptr)
      oldX.right->prev->succ = newZ.get();
    if (oldX.right->succ != nullptr)
      oldX.right->succ->prev = newZ.get();

    newX->parent = newZ.get();

    if (newX->left != nullptr)
      newX->left->parent = newX.get();
    if (newX->right != nullptr)
      newX->right->parent = newX.get();
    if (newZ->right != nullptr)
      newZ->right->parent = newZ.get();

    *parents_ptr = newZ;    // NOTE: oldX loses validity here, do not use
  }

  // pass in the parent's ptr to the node
  void rotate_right(std::shared_ptr<Node> *parents_ptr) {
    ASSERT((*parents_ptr)->left != nullptr, "left ptr cannot be null");

    Node &oldX = **parents_ptr;

    std::shared_ptr<Node> newX = std::make_shared<Node>(
      oldX.value,
      oldX.left->right,
      oldX.right,
      nullptr,    // equal to newZ
      oldX.prev,
      oldX.succ
    );

    if (oldX.prev != nullptr)
      oldX.prev->succ = newX.get();
    if (oldX.succ != nullptr)
      oldX.succ->prev = newX.get();

    std::shared_ptr<Node> newZ = std::make_shared<Node>(
      oldX.left->value,
      oldX.left->left,
      newX,
      oldX.parent,
      oldX.right->prev,
      oldX.right->succ
    );

    if (oldX.right->prev != nullptr)
      oldX.right->prev->succ = newZ.get();
    if (oldX.right->succ != nullptr)
      oldX.right->succ->prev = newZ.get();

    newX->parent = newZ.get();

    if (newX->left != nullptr)
      newX->left->parent = newX.get();
    if (newX->right != nullptr)
      newX->right->parent = newX.get();
    if (newZ->left != nullptr)
      newZ->left->parent = newZ.get();

    *parents_ptr = newZ;    // NOTE: oldX loses validity here, do not use
  }

  // pass in the parent's ptr to the node
  void rotate_if_necessary(std::shared_ptr<Node> *parents_ptr) {
    std::shared_ptr<Node> old_x = *parents_ptr;
    update_depth_and_size(**parents_ptr);

    if (!needs_rebalance(*old_x))
      return;

    const int x_on_left = old_x->left == nullptr ? 0 : old_x->left->depth;
    const int x_on_right = old_x->right == nullptr ? 0 : old_x->right->depth;

    const bool z_on_left = x_on_left > x_on_right;
    const int bf = get_balance(*(z_on_left ? old_x->left : old_x->right));

    if (!z_on_left && bf >= 0) {
      rotate_left(parents_ptr);
    } else if (z_on_left && bf <= 0) {
      rotate_right(parents_ptr);
    } else if (!z_on_left && bf < 0) {
      // rotate right around z
      if (z_on_left)
        rotate_right(&old_x->left);
      else
        rotate_right(&old_x->right);

      // rotate right around x
      rotate_left(parents_ptr);
    } else if (z_on_left && bf > 0) {
      // rotate left around z
      if (z_on_left)
        rotate_left(&old_x->left);
      else
        rotate_left(&old_x->right);

      // rotate right around x
      rotate_right(parents_ptr);
    }

    if ((*parents_ptr)->left != nullptr)
      update_depth_and_size(*(*parents_ptr)->left);
    if ((*parents_ptr)->right != nullptr)
      update_depth_and_size(*(*parents_ptr)->right);

    update_depth_and_size(**parents_ptr);
  }

  // pass in the parent's ptr to the node
  void internal_insert(
    std::shared_ptr<Node> ptr,
    T value,
    int position,
    Node *predecessor,
    Node *successor
  ) {
    const int on_left = ptr->left == nullptr ? 0 : ptr->left->size;

    Node *current_ptr = ptr.get();

    auto insert_or_recurse =
      [=, this](std::shared_ptr<Node> *parents_ptr, T value, int position) {
        if (*parents_ptr == nullptr) {
          ASSERT(position == 0, "Position should be zero");
          *parents_ptr =
            std::make_shared<Node>(value, current_ptr, predecessor, successor);

          if (predecessor != nullptr) {
            predecessor->succ = (*parents_ptr).get();
          }

          if (successor != nullptr)
            successor->prev = (*parents_ptr).get();

          rotate_if_necessary(parents_ptr);    // hope this works??
        } else {
          internal_insert(
            *parents_ptr, value, position, predecessor, successor
          );

          rotate_if_necessary(parents_ptr);    // hope this works??
        }
      };

    if (position <= on_left) {
      insert_or_recurse(&ptr->left, value, position);
    } else {
      insert_or_recurse(&ptr->right, value, position - on_left - 1);
    }
  }

  T internal_at(std::shared_ptr<Node> ptr, int position) const {
    int on_left = ptr->left == nullptr ? 0 : ptr->left->size;

    if (position < on_left)
      return internal_at(ptr->left, position);
    if (position == on_left)
      return ptr->value;
    return internal_at(ptr->right, position - on_left - 1);
  }
  // pass in the parent's ptr to the node
  void internal_erase(std::shared_ptr<Node> *ptr, int position) {
    std::shared_ptr<Node> curr = *ptr;

    int on_left = curr->left == nullptr ? 0 : curr->left->size;
    if (position < on_left) {
      internal_erase(&curr->left, position);
      rotate_if_necessary(ptr);
    } else if (position == on_left) {    // delete current
      Node *newSucc = curr->succ;
      Node *newPrev = curr->prev;

      // CASE 1: no children, just remove node and things will be updated
      if (curr->left == nullptr && curr->right == nullptr) {
        *ptr = nullptr;

      } else if (curr->left != nullptr && curr->right != nullptr) {
        // CASE 2: both children
        // replace curr with it's successor, delete successor

        std::shared_ptr<Node> *succ_ptr = &curr->right;
        while ((*succ_ptr)->left != nullptr)
          succ_ptr = &(*succ_ptr)->left;

        curr->value = (*succ_ptr)->value;

        internal_erase(ptr, on_left + 1);
      } else if (curr->left != nullptr) {    // CASE 3: only left, shift up
        std::shared_ptr<Node> temp = curr->left;
        *ptr = temp;
      } else {    // CASE 4: only right, shift up
        std::shared_ptr<Node> temp = curr->right;
        *ptr = temp;
      }

      if (newSucc != nullptr)
        newSucc->prev = newPrev;
      if (newPrev != nullptr)
        newPrev->succ = newSucc;
    } else {
      ASSERT(
        curr->right != nullptr, "Right pointer should not be null if recursing"
      );
      internal_erase(&(*ptr)->right, position - 1 - on_left);
      rotate_if_necessary(ptr);
    }
  }
};
