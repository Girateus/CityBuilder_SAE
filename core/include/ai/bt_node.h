#ifndef CORE_AI_BT_NODE_H
#define CORE_AI_BT_NODE_H

#include <algorithm>
#include <print>

namespace core::ai::behaviour_tree {

enum class Status { kFailure, kRunning, kSuccess };

// Base class for every node of the tree. Movable-only (rule of five):
// a node belongs to exactly one parent, so copying is forbidden.
class Node {
 public:
  Node() = default;
  virtual ~Node() = default;

  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;

  Node(Node&& node) noexcept{
      std::println("Move semantic move constructor");
      std::swap(status_, node.status_);
  }
  Node& operator=(Node&& node) noexcept {
      std::println("Move semantic move operator");
    std::swap(status_, node.status_);
    return *this;
  }

  virtual void Reset() = 0;
  virtual Status Tick() = 0;

 protected:
  Status status_ = Status::kFailure;
};

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_NODE_H
