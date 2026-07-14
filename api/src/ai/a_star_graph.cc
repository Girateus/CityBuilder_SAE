//
// Created by sebas on 11.06.2026.
//


#include "ai/a_star_graph.h"

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

#include "rng/rng.h"

namespace api::ai {
    void AStarGraph::AddNode(sf::Vector2i node){
      auto [_,was_inserted] = walkables_.emplace(node);
      if (was_inserted) {
        walkables_vec_.push_back(node);
      }
    }

    void AStarGraph::RemoveNode(const sf::Vector2i node){
      if (walkables_.erase(node)) {
        auto it = std::ranges::find(walkables_vec_, node);
        if (it != walkables_vec_.end()) {
          *it = walkables_vec_.back();
          walkables_vec_.pop_back();
        }
      }
    }

    bool AStarGraph::ContainsNode(const sf::Vector2i node) const{
        return walkables_.contains(node);
    }

    sf::Vector2i AStarGraph::GetRandomNode(){
      const size_t rnd_idx = core::rng::get_value(
     static_cast<size_t>(0), walkables_vec_.size() - 1);
      return walkables_vec_[rnd_idx];
    }

    std::vector<sf::Vector2i> AStarGraph::GetPath(sf::Vector2i start, sf::Vector2i end) const{
        std::priority_queue<AStarVertex, std::vector<AStarVertex>, VxCompareByF> open_queue;

        // came_from_ (a reused member) doubles as the closed set: a position is a key iff
        // it has been settled, and the stored value is the parent we reached it from. This
        // gives O(1) membership AND the reverse chain for path reconstruction.
        came_from_.clear();

        sf::Vector2i rounded_start = {start.x - (start.x % world_offset_.x),  start.y - (start.y % world_offset_.y)};
        sf::Vector2i rounded_end = {end.x - (end.x % world_offset_.x),  end.y - (end.y % world_offset_.y)};

        if (!walkables_.contains(rounded_start) || !walkables_.contains(rounded_end)) {
            return {};
        }

        // Start is its own parent; the reconstruction loop stops at rounded_start so it is never followed.
        open_queue.push(AStarVertex(rounded_start, 0, ManhattanDistance(rounded_start, rounded_end), rounded_start));

        while (!open_queue.empty()) {

            AStarVertex v = open_queue.top(); open_queue.pop();

            // Closed-set membership in O(1): skip if this position was already settled.
            if (came_from_.contains(v.position)) {
                continue;
            }
            came_from_.emplace(v.position, v.parent_position);

            if (v.position == rounded_end){
                // Walk the reverse chain from end back to start (start excluded), then reverse.
                std::vector<sf::Vector2i> path;
                for (sf::Vector2i cur = rounded_end; cur != rounded_start; cur = came_from_.at(cur)) {
                    path.push_back(cur);
                }
                std::ranges::reverse(path);
                return path;
            }

            for (sf::Vector2i neighbour: kNeighbours) {
                sf::Vector2i new_position = v.position + sf::Vector2i{neighbour.x * world_offset_.x, neighbour.y * world_offset_.y};
                if (!came_from_.contains(new_position) && walkables_.contains(new_position)) {
                    open_queue.push(AStarVertex(new_position, v.g + world_offset_.x, ManhattanDistance(new_position, rounded_end), v.position));
                }
            }
        }

        return {};
    }

    int ManhattanDistance(sf::Vector2i a, sf::Vector2i b){
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }
}
