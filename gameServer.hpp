
#pragma once
#include "gameCommon.hpp"
#include <vector>

namespace Game::Server {
    class PlayerBoard {
    public:
        std::vector<Game::Ship> ships;
        std::vector<Game::Shot> shots;
        const int width;
        const int height;

        PlayerBoard(int boardWidth, int boardHeight);

        bool AddShip(const Game::Ship& ship);

        bool TakeShot(Game::Coords pos);

        bool Shoot(Game::Coords pos, PlayerBoard& other);

        bool IsDead();
    };
}

