#include <optional>
#include <filesystem>
#include <cmath>

#include "game.h"
#include "SFML/Graphics.hpp"

#include "ai/a_star_graph.h"
#include "ai/npc.h"
#include "ai/npc_manager.h"
#include "graphics/camera.h"
#include "loader.h"
#include "saver.h"
#include "tilemap.h"
#include "ui/button_maker.h"
#include "ui/ui_manager.h"

namespace game {
namespace {
constexpr sf::Vector2i world_size    = {1920 * 2 / 32, 1080 * 2 / 32};
constexpr sf::Vector2i world_offset  = {32, 32};
constexpr sf::Vector2f window_size_f = {1920.f, 1080.f};
constexpr sf::Vector2u window_size_u = {1920u, 1080u};

sf::Clock        clock_;
sf::RenderWindow window_;
bool             isFullscreen_ = false;

Tilemap             map_;
Saver               saver;
graphics::Camera    camera_;
api::ai::NPCManager npc_manager_;
api::ai::AStarGraph astar_graph_{world_size, world_offset};

api::ui::ui_manager  ui_manager_;
std::optional<HouseTile> selected_house_;
bool               menu_open_ = false;
sf::RectangleShape preview_shape_{{32.f, 32.f}};

bool IsPlaceable(sf::Vector2f world_pos) {
    return map_.IsWalkable(world_pos);
}

void PlaceHouse(sf::Vector2f world_pos) {
  if (!selected_house_) return;

  // ← Snapper sur la grille
  constexpr float kOffset = 32.f;
  world_pos.x = std::floor(world_pos.x / kOffset) * kOffset;
  world_pos.y = std::floor(world_pos.y / kOffset) * kOffset;

  if (!IsPlaceable(world_pos)) return;

  map_.PlaceHouse(world_pos, *selected_house_);
  npc_manager_.SpawnNPC(  astar_graph_, *selected_house_,
    world_pos,[&](sf::Vector2f pos, ResourceTile type) {
      return map_.ReserveNearestResource(pos, type); // ← réserve au lieu de juste chercher
  },
  [&](sf::Vector2f pos) {
      map_.RemoveResource(pos, astar_graph_); // ← passe le graph pour réajouter le nœud
  },
  [&](sf::Vector2f pos) {
      map_.UnreserveResource(pos); // ← nouveau callback
  }
  );
  selected_house_ = std::nullopt;
}

void CloseMenu() {
    menu_open_ = false;
    ui_manager_.SetVisible(1, false);
    ui_manager_.SetVisible(2, false);
    ui_manager_.SetVisible(3, false);
}

void BuildUI() {
    api::ui::ButtonMaker builder;

    // ── Bouton Build (ouvre/ferme le menu) ──────────────────────────────
    ui_manager_.Register(
        builder.New()
        .WithPosition({window_size_f.x - 110.f, 10.f}, {96.f, 96.f})
        .WithText("Build")
        .WithBaseTile(sf::FloatRect({0,  3 * 48}, {48, 48}))
        .WithHoverTile(sf::FloatRect({48, 3 * 48}, {48, 48}))
        .WithClickTile(sf::FloatRect({48, 0 * 48}, {48, 48}))
        .WithClickCallback([]() {
            menu_open_ = !menu_open_;
            ui_manager_.SetVisible(1, menu_open_);
            ui_manager_.SetVisible(2, menu_open_);
            ui_manager_.SetVisible(3, menu_open_);
        })
        .Build()
    );

    // ── Bouton Lumberjack ────────────────────────────────────────────────
    ui_manager_.Register(
        builder.New()
        .WithPosition({window_size_f.x - 110.f, 116.f}, {96.f, 96.f})
        .WithText("Lumberjack")
        .WithBaseTile(sf::FloatRect({0,  3 * 48}, {48, 48}))
        .WithHoverTile(sf::FloatRect({48, 3 * 48}, {48, 48}))
        .WithClickTile(sf::FloatRect({48, 0 * 48}, {48, 48}))
        .WithClickCallback([]() {
            selected_house_ = HouseTile::kLumberjack;
            CloseMenu();
        })
        .Build()
    );

    // ── Bouton Gatherer ──────────────────────────────────────────────────
    ui_manager_.Register(
        builder.New()
        .WithPosition({window_size_f.x - 110.f, 222.f}, {96.f, 96.f})
        .WithText("Gatherer")
        .WithBaseTile(sf::FloatRect({0,  3 * 48}, {48, 48}))
        .WithHoverTile(sf::FloatRect({48, 3 * 48}, {48, 48}))
        .WithClickTile(sf::FloatRect({48, 0 * 48}, {48, 48}))
        .WithClickCallback([]() {
            selected_house_ = HouseTile::kGatherer;
            CloseMenu();
        })
        .Build()
    );

    // ── Bouton Miner ─────────────────────────────────────────────────────
    ui_manager_.Register(
        builder.New()
        .WithPosition({window_size_f.x - 110.f, 328.f}, {96.f, 96.f})
        .WithText("Miner")
        .WithBaseTile(sf::FloatRect({0,  3 * 48}, {48, 48}))
        .WithHoverTile(sf::FloatRect({48, 3 * 48}, {48, 48}))
        .WithClickTile(sf::FloatRect({48, 0 * 48}, {48, 48}))
        .WithClickCallback([]() {
            selected_house_ = HouseTile::kMiner;
            CloseMenu();
        })
        .Build()
    );

    CloseMenu();

    preview_shape_.setFillColor(sf::Color(255, 255, 255, 150));
    preview_shape_.setOutlineColor(sf::Color::White);
    preview_shape_.setOutlineThickness(2.f);
}

void Setup() {
    window_.create(sf::VideoMode(window_size_u), "SFML window", sf::Style::Default);
    camera_.Setup(window_size_f);

    map_.Setup(world_size, {world_offset.x, world_offset.y}, astar_graph_);

    const std::filesystem::path save_path = "save/map.sav";

    if (std::filesystem::exists(save_path)) {
        astar_graph_ = api::ai::AStarGraph{world_size, world_offset};
        Loader loader;
        map_.Load(save_path, loader, static_cast<sf::Vector2f>(world_offset), astar_graph_);
    } else {
        std::filesystem::create_directories(save_path.parent_path());
        map_.Save(save_path, saver);
    }

    npc_manager_.Setup(
    "_assets/Assets_Game_prog_Carusone_Matheo_2025_10_22/collecteur-de-bois.png",    // lumberjack
    "_assets/Assets_Game_prog_Carusone_Matheo_2025_10_22/collecteur-de-nouriture.png",  // gatherer
    "_assets/Assets_Game_prog_Carusone_Matheo_2025_10_22/collecteur-de-pierre.png",  // miner ← adapter
    world_size);

    /*for (int i = 0; i < 500; ++i) {
        npc_manager_.SpawnNPC(astar_graph_);
    }*/

    // UI
    ui_manager_.InitTextures("_assets/UI/");

    BuildUI();
}

void ToggleFullscreen() {
    isFullscreen_ = !isFullscreen_;
    if (isFullscreen_) {
        window_.create(sf::VideoMode::getDesktopMode(), "RoboBuilder", sf::State::Fullscreen);
    } else {
        window_.create(sf::VideoMode(window_size_u), "RoboBuilder", sf::Style::Default);
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
                if (key->code == sf::Keyboard::Key::Escape) {
                    if (selected_house_.has_value()) {
                        // Annule le placement en cours
                        selected_house_ = std::nullopt;
                        CloseMenu();
                    } else {
                        window_.close();
                    }
                }
            }

            if (selected_house_.has_value()) {
                if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (click->button == sf::Mouse::Button::Left) {
                        const sf::Vector2f world_pos =
                            window_.mapPixelToCoords(click->position);
                        PlaceHouse(world_pos);
                    }
                }
            } else {
                ui_manager_.HandleEvent(*event, window_);
            }

            camera_.HandleEvent(*event, window_);
        }

        if (selected_house_.has_value()) {
            const sf::Vector2f world_pos =
                window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
            const float tx = std::floor(world_pos.x / world_offset.x) * world_offset.x;
            const float ty = std::floor(world_pos.y / world_offset.y) * world_offset.y;
            preview_shape_.setPosition({tx, ty});
            preview_shape_.setFillColor(
                IsPlaceable({tx + 1.f, ty + 1.f})
                ? sf::Color(0, 255, 0, 120)
                : sf::Color(255, 0, 0, 120));
        }

        camera_.Update(dt);
        camera_.Apply(window_);
        npc_manager_.Update(dt);

        window_.clear();
        map_.Draw(window_);
        npc_manager_.Draw(window_);

        // Preview en espace monde
        if (selected_house_.has_value()) {
            window_.draw(preview_shape_);
        }

        // UI en espace écran (vue fixe, indépendante de la caméra)
        sf::View ui_view;
        ui_view.setSize(window_size_f);
        ui_view.setCenter(window_size_f * 0.5f);
        window_.setView(ui_view);
        ui_manager_.Draw(window_);

        window_.display();
    }
}

}  // namespace game