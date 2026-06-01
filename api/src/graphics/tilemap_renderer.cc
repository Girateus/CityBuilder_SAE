//
// Created by sebas on 07.05.2026.
//

#include "graphics/tilemap_renderer.h"

namespace graphics {
    void TilemapRenderer::AddTile(sf::Vector2f pos, sf::Vector2f offset, sf::FloatRect texBounds){
        vertices_.append(sf::Vertex(pos, color, texBounds.position));
        vertices_.append(sf::Vertex(pos + sf::Vector2f(offset.x, 0.f), color,
                                    texBounds.position + sf::Vector2f(texBounds.size.x, 0.f))); // + X
        vertices_.append(sf::Vertex(pos + offset, color, texBounds.position + texBounds.size));

        vertices_.append(sf::Vertex(pos, color, texBounds.position));
        vertices_.append(sf::Vertex(pos + offset, color, texBounds.position + texBounds.size));;
        vertices_.append(sf::Vertex(pos + sf::Vector2f(0.f, offset.y), color,
                                    texBounds.position + sf::Vector2f(0.f, texBounds.size.y))); // + Y
    }

    void TilemapRenderer::Draw(sf::RenderWindow &window) const{
        sf::RenderStates states;
        if (texture_) {
            states.texture = texture_;
        }
        if (vertices_.getVertexCount() > 0) {
            window.draw(vertices_, states);
        }
    }

    void TilemapRenderer::SetTexture(sf::Texture *texture){
        texture_ = texture;
    }
    void TilemapRenderer::ClearVertices(){
        vertices_.clear();
    }

} // namespace graphics
