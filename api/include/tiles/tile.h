//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILE_H
#define CITYBUILDER_TILE_H
#include <SFML/System/Vector2.hpp>
#include <type_traits>

namespace tiles {

template <typename T>
  requires std::is_enum_v<T>
struct Tile {
  sf::Vector2f pos;
  T type;
};

}  // namespace tiles

#endif  // CITYBUILDER_TILE_H
