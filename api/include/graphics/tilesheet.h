//
// Created by sebas on 07.05.2026.
//

#ifndef CITYBUILDER_TILESHEET_H
#define CITYBUILDER_TILESHEET_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string_view>
#include <unordered_map>

namespace graphics {
    template<typename T>
    class Tilesheet {
        std::unordered_map<T, sf::FloatRect> tile_rects;
        sf::Texture texture;
        int tile_step_ = 0;

        static sf::FloatRect ConstructRect(float xIdx, float yIdx, float width, float height){
            return {{width * xIdx, yIdx * height}, {width, height}};
        }

    public:
        sf::Texture *GetTexture(){ return &texture; }

        bool InitTileSheet(std::string_view path, int tile_step){
            tile_step_ = tile_step;
            return texture.loadFromFile(path);
        }

        void AddTile(T type, int xIdx, int yIDx){
            tile_rects[type] = ConstructRect(
                static_cast<float>(xIdx), static_cast<float>(yIDx),
                static_cast<float>(tile_step_), static_cast<float>(tile_step_));
        }

        sf::FloatRect GetBounds(T type){
            if (tile_rects.contains(type)) {
                return tile_rects.at(type);
            }
            return {};
        }
    };
} // namespace graphics

#endif  // CITYBUILDER_TILESHEET_H
