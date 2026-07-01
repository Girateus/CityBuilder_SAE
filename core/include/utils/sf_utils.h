//
// Created by noahs on 30.06.2026.
//

#ifndef CITYBUILDER_SF_UTILS_H
#define CITYBUILDER_SF_UTILS_H

namespace core::utils {

struct Vec2iHash {
  size_t operator()(const sf::Vector2i node) const noexcept{
    return std::bit_cast<size_t>(node);
  }
};

}

#endif  // CITYBUILDER_SF_UTILS_H
