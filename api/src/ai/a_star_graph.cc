//
// Created by noahs on 17.06.2026.
#include "ai/a_star_graph.h"

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

#include "rng/rng.h"

namespace api::ai {
void AStarGraph::AddNode(sf::Vector2i node){
  walkables_.emplace(node);
}

void AStarGraph::RemoveNode(const sf::Vector2i node){
  walkables_.erase(node);
}

bool AStarGraph::ContainsNode(const sf::Vector2i node) const{
  return walkables_.contains(node);
}

const sf::Vector2i AStarGraph::GetRandomNode(){
  int rnd_idx = core::rng::get_value(static_cast<size_t>(0), walkables_.size() - 1);
  // Accessing third element
  auto it = next(walkables_.begin(), rnd_idx);
  return {it->x, it->y};
}

std::vector<sf::Vector2i> AStarGraph::GetPath(sf::Vector2i start, sf::Vector2i end){
  std::vector<sf::Vector2i> path;

  std::priority_queue<AStarVertex, std::vector<AStarVertex>, std::greater<>> open_queue;
  visited_vertices.clear();

  sf::Vector2i rounded_start = {start.x - (start.x % world_offset_.x),  start.y - (start.y % world_offset_.y)};
  sf::Vector2i rounded_end = {end.x - (end.x % world_offset_.x),  end.y - (end.y % world_offset_.y)};

  if (!walkables_.contains(rounded_start) || !walkables_.contains(rounded_end)) {
    return {};
  }

  open_queue.push(AStarVertex(rounded_start, 0, ManhattanDistance(start, end), -1));

  while (!open_queue.empty()) {

    AStarVertex v = open_queue.top(); open_queue.pop();

    // Check doubled positions in open queue
    auto visited = std::ranges::find_if(visited_vertices, [v](AStarVertex vCheck){return vCheck.position == v.position;});
    if (visited != visited_vertices.end()) {
      continue;
    }
    visited_vertices.push_back(v);
    size_t visited_idx = visited_vertices.size() - 1;

    if (v.position == end){
      // Research parents + reverse
      auto pathVertex = v;
      while (pathVertex.parent_idx > -1) {
        path.push_back(pathVertex.position);
        pathVertex = visited_vertices[pathVertex.parent_idx];
      }
      std::ranges::reverse(path);
      return path;
    }

    for (sf::Vector2i neighbour: kNeighbours) {
      sf::Vector2i new_position = v.position + sf::Vector2i{neighbour.x * world_offset_.x, neighbour.y * world_offset_.y};

      auto visited = std::ranges::find_if(visited_vertices, [new_position](AStarVertex v){return v.position == new_position;});

      int distance_from_start = ManhattanDistance(new_position, start);
      if (visited == visited_vertices.end() && walkables_.contains(new_position) && distance_from_start <= 200) {
        open_queue.push(AStarVertex(new_position, v.g + 1, ManhattanDistance(new_position, end), visited_idx));
      }

    }

  }

  // path.push_back(start);
  // path.push_back(end);

  return path;
}

int ManhattanDistance(sf::Vector2i a, sf::Vector2i b){
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
}
