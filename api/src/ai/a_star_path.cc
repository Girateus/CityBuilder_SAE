//
// Created by noahs on 01.07.2026.
//

#include "ai/a_star_path.h"

void AStarPath::SetPath(const std::vector<sf::Vector2i>& path) {
  path_idx_ = 0;

  if (path.empty()) {
    path_.clear();
    is_valid_ = false;
    is_ready_ = true;
  } else {
    path_ = std::vector(path);
    is_valid_ = true;
    is_ready_ = true;
  }
}

void AStarPath::NextPosition() {
  if (path_idx_ < path_.size()) {
      path_idx_++;
  }
}

sf::Vector2i AStarPath::CurrentPosition() const {
  if (path_idx_ < path_.size()) {
    return path_[path_idx_];return sf::Vector2i{path_[path_idx_]};
  }
  return sf::Vector2i{path_[path_.size() - 1]};
}

bool AStarPath::IsGoalReached() const {
  return path_idx_ >= path_.size();
}

bool AStarPath::IsReady() const {
  return is_ready_;
}

bool AStarPath::IsValid() const {
  return is_valid_;
}