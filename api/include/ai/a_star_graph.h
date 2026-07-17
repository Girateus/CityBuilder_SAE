//
// Created by sebas on 11.06.2026.
//

#ifndef CITYBUILDER_PATH_H
#define CITYBUILDER_PATH_H

#include <array>
#include <bit>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "utils/sf_utils.h"

namespace api::ai {



    struct AStarVertex {
        sf::Vector2i position = sf::Vector2i(-1, -1);
        int g = 0; // Dijkstra cost
        int h = 0; // Heuristic value
        // Position we reached this vertex from; recorded into the came_from map when settled.
        sf::Vector2i parent_position = sf::Vector2i(-1, -1);

        [[nodiscard]] int F() const{return g + h;};

        // Ordering for the open-queue (min-heap via std::greater).
        //bool operator>(const AStarVertex& other) const{return F() > other.F();};

    };

    struct VxCompareByF {
        bool operator()(AStarVertex a, AStarVertex b)
        {
            return a.F() < b.F();
        }
    };

    class AStarGraph {
        //TODO change to std::map with index
        std::unordered_set<sf::Vector2i, core::utils::Vec2iHash> walkables_;
        std::vector<sf::Vector2i> walkables_vec_;
        sf::Vector2i world_offset_;

        // Scratch reused across GetPath calls: doubles as the closed set (key = settled)
        // and the reverse parent chain (value = parent). mutable so the const GetPath can
        // clear and fill it; clear() keeps capacity, so steady-state pathing reallocates nothing.
        mutable std::unordered_map<sf::Vector2i, sf::Vector2i, core::utils::Vec2iHash> came_from_; //mutable pas bien



    public:
        explicit AStarGraph(sf::Vector2i world_size, sf::Vector2i world_offset) : world_offset_(world_offset){
        };

        void AddNode(sf::Vector2i node);
        void RemoveNode(sf::Vector2i node);
        [[nodiscard]] bool ContainsNode(sf::Vector2i node) const;

        sf::Vector2i GetRandomNode();

        [[nodiscard]] std::vector<sf::Vector2i> GetPath(sf::Vector2i start, sf::Vector2i end) const;

    };



    constexpr std::array kNeighbours{
        sf::Vector2i(0, -1), // NORTH
        sf::Vector2i(1, 0), // EAST
        sf::Vector2i(0, 1), // SOUTH
        sf::Vector2i(-1, 0) // WEST
    };

    int ManhattanDistance(sf::Vector2i a, sf::Vector2i b);

}
#endif //CITYBUILDER_PATH_H
