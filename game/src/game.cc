#include <optional>
#include "game.h"


#include <filesystem>


#include "SFML/Graphics.hpp"

#include "ai/a_star_graph.h"
#include "ai/npc.h"
#include "ai/npc_manager.h"
#include "graphics/camera.h"
#include "loader.h"
#include "saver.h"
#include "tilemap.h"
#include "ui/button_maker.h"
//#include "ui/ui_manager.h"

namespace game {
namespace {
constexpr sf::Vector2i world_size   = {1920 *2 / 32, 1080 *2 / 32};
constexpr sf::Vector2i world_offset = {32, 32};
constexpr sf::Vector2f window_size_f = {1920.f, 1080.f};
constexpr sf::Vector2u window_size_u = {1920u, 1080u};

sf::Clock        clock_;
sf::RenderWindow window_;
bool             isFullscreen_ = false;

Tilemap          map_;
Saver saver;
graphics::Camera camera_;
api::ai::NPCManager npc_manager_;
// AStarGraph construit en même temps que la tilemap dans Setup()
api::ai::AStarGraph astar_graph_{world_size, world_offset};

//api::ui::ui_manager ui_manager_;

void Setup() {
    window_.create(sf::VideoMode(window_size_u), "SFML window", sf::Style::Default);
    camera_.Setup(window_size_f);

    // Setup remplit astar_graph_ en même temps qu'il génère le terrain
  map_.Setup(world_size, {world_offset.x, world_offset.y}, astar_graph_);

  /*ui_manager_.InitTextures("");
  ui_manager_.InitLabelStyle("");*/

  //api::ui::ButtonBuilder

  const std::filesystem::path save_path = "save/map.sav";

  if (std::filesystem::exists(save_path)) {
    // Recrée le graphe vide avant de le reremplir via Load
    astar_graph_ = api::ai::AStarGraph{world_size, world_offset};

    Loader loader;
    //converti Vector2i de world_offset en Vector2f
    map_.Load(save_path, loader, static_cast<sf::Vector2f>(world_offset), astar_graph_);
  } else {

    std::filesystem::create_directories(save_path.parent_path());

    map_.Save(save_path, saver);
  }

    npc_manager_.Setup("_assets/Assets_Game_prog_Carusone_Matheo_2025_10_22/collecteur-de-bois.png", world_size);

  for (int i = 0; i < 500; ++i) {
    npc_manager_.SpawnNPC(astar_graph_);
  }
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

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
              if (key->code == sf::Keyboard::Key::Escape) {
                // TODO : implement save when closing game
                //map_.Save(save_path, saver);
                window_.close();
              }
            }
            camera_.HandleEvent(*event, window_);
        }

        camera_.Update(dt);
        camera_.Apply(window_);
        npc_manager_.Update(dt);

        window_.clear();
        map_.Draw(window_);
        npc_manager_.Draw(window_);
        window_.display();
    }
}

}  // namespace game