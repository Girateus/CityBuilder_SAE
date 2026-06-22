#include "game.h"

#include <filesystem>
#include <optional>

#include "SFML/Graphics.hpp"
#include "ai/a_star_graph.h"
#include "ai/npc.h"
#include "graphics/camera.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"
#include "tilemap.h"
#include "saver.h"
#include "loader.h"

namespace game {
namespace {
constexpr sf::Vector2i grid_size   = {1920 * 5 / 32, 1080 * 5 / 32}; // en tiles
constexpr sf::Vector2f grid_offset = {32.f, 32.f};
constexpr sf::Vector2f window_size_f = {1920.f, 1080.f};
constexpr sf::Vector2u window_size_u = {1920u, 1080u};

sf::Clock        clock_;
sf::RenderWindow window_;
bool             isFullscreen_ = false;

// AStarGraph construit en même temps que la tilemap dans Setup()
api::ai::AStarGraph astar_graph_{grid_size, sf::Vector2i{grid_offset}};

Tilemap          map_;
graphics::Camera camera_;

sf::Texture      npc_texture_;
api::ai::Npc     npc_;

void Setup() {
    window_.create(sf::VideoMode(window_size_u), "SFML window", sf::Style::Default);
    camera_.Setup(window_size_f);

    // Setup remplit astar_graph_ en même temps qu'il génère le terrain
    map_.Setup(grid_size, grid_offset, astar_graph_);

  const std::filesystem::path save_path = "save/map.sav";

  if (std::filesystem::exists(save_path)) {
    // Recrée le graphe vide avant de le reremplir via Load
    astar_graph_ = api::ai::AStarGraph{grid_size, sf::Vector2i{grid_offset}};

    Loader loader;
    map_.Load(save_path, loader, grid_offset, astar_graph_);
  } else {
    map_.Setup(grid_size, grid_offset, astar_graph_);

    std::filesystem::create_directories(save_path.parent_path());
    Saver saver;
    map_.Save(save_path, saver);
  }

    // Texture chargée ici, pointeur passé au NPC
    npc_texture_.loadFromFile(
        "_assets/Assets_Game_prog_Carusone_Matheo_2025_10_22/collecteur-de-pierre.png");

    npc_.Setup(&npc_texture_, grid_size, {100, 100}, astar_graph_);
}

void ToggleFullscreen() {
    isFullscreen_ = !isFullscreen_;
    if (isFullscreen_) {
        window_.create(sf::VideoMode::getDesktopMode(), "SFML window", sf::State::Fullscreen);
    } else {
        window_.create(sf::VideoMode(window_size_u), "SFML window", sf::Style::Default);
    }
    camera_.OnWindowResized(window_.getSize());
}

}  // namespace

void Loop() {
    Setup();

    while (window_.isOpen()) {
        const float dt = clock_.restart().asSeconds();

        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Enter && key->alt) {
                    ToggleFullscreen();
                    continue;
                }
            }
            camera_.HandleEvent(*event, window_);
        }

        camera_.Update(dt);
        camera_.Apply(window_);
        npc_.Update(dt);

        window_.clear();
        map_.Draw(window_);
        npc_.Draw(window_);
        window_.display();
    }
}

}  // namespace game