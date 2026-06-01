//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_H
#define CITYBUILDER_TILEMAP_H

#include "FastNoiseLite.h"
#include "game_types.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"

class Tilemap {

    graphics::TilemapRenderer terrain_renderer_;
    graphics::Tilesheet<TerrainTiles> terrain_tilesheet_;

    graphics::TilemapRenderer ressources_renderer_;
    graphics::Tilesheet<RessourcesTiles> ressources_tilesheet_;

public:
    void Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset);
    void Draw(sf::RenderWindow &window);

};



// void GenerateTilemap(){
//
//     FastNoiseLite noise;
//
//     noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
//     noise.SetFrequency(10);
//     noise.SetFractalOctaves(1);
//     noise.SetFractalLacunarity(2);
//     noise.SetFractalGain(0.5);
//     noise.GetNoise(1.f,1.f);
//
// }

#endif //CITYBUILDER_TILEMAP_H
