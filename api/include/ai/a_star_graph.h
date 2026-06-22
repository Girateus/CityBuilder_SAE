//
// Created by noahs on 17.06.2026.
//

#ifndef CITYBUILDER_PATH_H
#define CITYBUILDER_PATH_H

#include <array>
#include <unordered_set>
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace api::ai {

    // TODO : Slide about hash and unordered_set
    struct Vec2iHash {
        size_t operator()(const sf::Vector2i& node) const noexcept{
            size_t h = std::hash<int>{}(node.x);
            h ^= std::hash<int>{}(node.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
    //
    // // TODO : Slide about hash and unordered_set
    // struct Vec2iEq {
    //     bool operator()(const sf::Vector2i& a,const sf::Vector2i& b) const noexcept{return a.x == b.x && a.y == b.y;}
    // };

    struct AStarVertex {
        sf::Vector2i position = sf::Vector2i(-1, -1);
        int g = 0; // Dijkstra cost
        int h = 0; // Heuristic value
        // AStarVertex* parent = nullptr;
        int parent_idx = -1;

        [[nodiscard]] int F() const{return g + h;};

        bool operator>(const AStarVertex& other) const{return F() > other.F();};
        bool operator==(const AStarVertex &other) const{return position == other.position && F() == other.F();};

        struct AStarVertexHash {
            size_t operator()(const AStarVertex& vertex) const noexcept{
                return Vec2iHash{}(vertex.position);
            }
        };

    };

    class AStarGraph {

        // TODO : Slide about hash
        std::unordered_set<sf::Vector2i, Vec2iHash> walkables_;
        std::vector<AStarVertex> visited_vertices;
        sf::Vector2i world_offset_;

    public:
        explicit AStarGraph(sf::Vector2i world_size, sf::Vector2i world_offset) : world_offset_(world_offset){
            visited_vertices.reserve(world_size.x * world_size.y);
        };

        void AddNode(sf::Vector2i node);
        void RemoveNode(sf::Vector2i node);
        [[nodiscard]] bool ContainsNode(sf::Vector2i node) const;
        const sf::Vector2i GetRandomNode();

        std::vector<sf::Vector2i> GetPath(sf::Vector2i start, sf::Vector2i end);

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

