//
// Created by noahs on 01.07.2026.
//

#ifndef CITYBUILDER_A_STAR_PATH_H
#define CITYBUILDER_A_STAR_PATH_H
#include <vector>
#include <SFML/System/Vector2.hpp>

class AStarPath {

  std::vector<sf::Vector2i> path_;
  int path_idx_ {0};
  bool is_ready_ {false};
  bool is_valid_ {false};

  public:
    void SetPath(const std::vector<sf::Vector2i>& path);
    void NextPosition();
    [[nodiscard]] sf::Vector2i CurrentPosition() const;
    [[nodiscard]] bool IsGoalReached() const;
    [[nodiscard]]bool IsReady() const;
    [[nodiscard]]bool IsValid() const;
};


#endif  // CITYBUILDER_A_STAR_PATH_H
