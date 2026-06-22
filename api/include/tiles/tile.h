//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILE_H
#define CITYBUILDER_TILE_H
#include <type_traits>
#include <SFML/System/Vector2.hpp>

namespace api::tiles {

struct BasicTile {
  sf::Vector2f  Pos;
  bool IsWalkable = false;
};

template<typename T>
requires std::is_enum_v<T>
struct Tile : BasicTile {
  T type;
};


}

#endif //CITYBUILDER_TILE_H