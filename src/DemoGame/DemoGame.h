#ifndef GAME_H
#define GAME_H

#include "Engine/Game.h"
#include "Engine/Scene.h"
#include "Engine/Entity.h"
#include "Engine/Components.h"
#include "Engine/Systems.h"
#include "Colliders.h"
#include "Player.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "Render.h"
#include <entt/entt.hpp>

const int WIDTH = 1024;
const int HEIGHT = 768;

class MovementSystem : public UpdateSystem {
public:
    void run(float dT) {
        auto view = scene->r.view<PositionComponent, VelocityComponent>();
        for (auto e : view) {
            auto& pos = view.get<PositionComponent>(e);
            auto vel = view.get<VelocityComponent>(e);
            pos.x += (vel.x * dT);
            pos.y += (vel.y * dT);
        }
    }
};

class WallHitSystem : public UpdateSystem {
public:
    void run(float dT) {
        auto view = scene->r.view<PositionComponent, VelocityComponent, SpriteComponent>();
        for (auto e : view) {
            auto pos = view.get<PositionComponent>(e);
            auto spr = view.get<SpriteComponent>(e);
            auto& vel = view.get<VelocityComponent>(e);
            int newPosX = pos.x + vel.x * dT;
            int newPosY = pos.y + vel.y * dT;
            if (newPosX < 0 || newPosX + spr.width > 1024) {
                vel.x *= -1;
            }
            if (newPosY < 0 || newPosY + spr.height > 768) {
                vel.y *= -1;
            }
        }
    }
};

class LoseSystem : public UpdateSystem {
public:
    void run(float dT) {
        auto view = scene->r.view<PositionComponent, VelocityComponent, SpriteComponent>();
        for (auto e : view) {
            auto pos = view.get<PositionComponent>(e);
            auto spr = view.get<SpriteComponent>(e);
            auto& vel = view.get<VelocityComponent>(e);
            int newPosX = pos.x + vel.x * dT;
            int newPosY = pos.y + vel.y * dT;
            if (newPosY + spr.height > 768) {
                std::cout << "Perdiste" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
    }
};

class EnemyCheckSystem : public UpdateSystem {
public:
    void run(float dT) override {
        auto view = scene->r.view<EnemyCollider>();

        bool allEnemiesEliminated = true;
        for (auto e : view) {
            auto& enemy = view.get<EnemyCollider>(e);
            if (!enemy.isDestroyed) {
                allEnemiesEliminated = false;
                break;
            }
        }

        if (allEnemiesEliminated) {
            std::cout << "Ganaste" << std::endl;
            std::exit(EXIT_SUCCESS); // Terminar el programa con un código de éxito
        }
    }
};

class SquareSpawnSetupSystem : public SetupSystem {
public:
    void run() {
        Entity* square = scene->createEntity("SQUARE", WIDTH/2, HEIGHT - 40);
        square->addComponent<BarCollider>(200, 30);
        square->addComponent<SpriteComponent>(200, 30, SDL_Color{255, 0, 0});
        square->addComponent<VelocityComponent>(0, 0);
        square->addComponent<PlayerComponent>(true, 500, 500);

        Entity* square2 = scene->createEntity("SQUARE2", 100, 0);
        square2->addComponent<ColliderComponent>(100, 100, false, true);
        square2->addComponent<VelocityComponent>(-400, 400);
        square2->addComponent<SpriteComponent>(100, 100, SDL_Color{0, 0, 255});

        Entity* enemy = scene->createEntity("Enemy", 0, 0);
        enemy->addComponent<SpriteComponent>(100, 50, SDL_Color{255, 0, 0});
        enemy->addComponent<EnemyCollider>(false);

        Entity* enemy2 = scene->createEntity("Enemy", 150, 0);
        enemy2->addComponent<SpriteComponent>(100, 50, SDL_Color{0, 255, 0});
        enemy2->addComponent<EnemyCollider>(false);

        Entity* enemy3 = scene->createEntity("Enemy", 300, 0);
        enemy3->addComponent<SpriteComponent>(100, 50, SDL_Color{0, 0, 255});
        enemy3->addComponent<EnemyCollider>(false);

        Entity* enemy4 = scene->createEntity("Enemy", 450, 0);
        enemy4->addComponent<SpriteComponent>(100, 50, SDL_Color{0, 0, 255});
        enemy4->addComponent<EnemyCollider>(false);

        Entity* enemy5 = scene->createEntity("Enemy", 600, 0);
        enemy5->addComponent<SpriteComponent>(100, 50, SDL_Color{0, 0, 255});
        enemy5->addComponent<EnemyCollider>(false);

        Entity* enemy6 = scene->createEntity("Enemy", 750, 0);
        enemy6->addComponent<SpriteComponent>(100, 50, SDL_Color{0, 0, 255});
        enemy6->addComponent<EnemyCollider>(false);
    }
};

class SquareRenderSystem : public RenderSystem {
public:
    void run(SDL_Renderer* renderer) {
        auto view = scene->r.view<PositionComponent, SpriteComponent>();
        for (auto e : view) {
            auto pos = view.get<PositionComponent>(e);
            auto spr = view.get<SpriteComponent>(e);
            SDL_SetRenderDrawColor(renderer, spr.color.r, spr.color.g, spr.color.b, spr.color.a);
            SDL_Rect r = {pos.x, pos.y, spr.width, spr.height};
            SDL_RenderFillRect(renderer, &r);
        }
    }
};

class DemoGame : public Game {
public:
    Scene* sampleScene;
    entt::registry r;

public:
    DemoGame()
        : Game("SAMPLE", WIDTH, HEIGHT) { }

    void setup() {
        std::cout << "HELLO WORLD";
        sampleScene = new Scene("SAMPLE SCENE", r);
        addSetupSystem<SquareSpawnSetupSystem>(sampleScene);
        addUpdateSystem<ColliderResetSystem>(sampleScene);
        addUpdateSystem<PlayerColliderSystem>(sampleScene);
        addUpdateSystem<LoseSystem>(sampleScene);
        addUpdateSystem<PlayerWallHitSystem>(sampleScene);
        addUpdateSystem<EnemyCollisionSystem>(sampleScene);
        addUpdateSystem<WallHitSystem>(sampleScene);
        addEventSystem<PlayerMovementSystem>(sampleScene);
        addUpdateSystem<MovementSystem>(sampleScene);
        addRenderSystem<SquareRenderSystem>(sampleScene);
        setScene(sampleScene);
    }
};

#endif // GAME_H
