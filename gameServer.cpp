
#include "gameCommon.hpp"
#include "gameServer.hpp"

namespace Game::Server {

    PlayerBoard::PlayerBoard(int boardWidth, int boardHeight)
        : width(boardWidth), height(boardHeight) {}

    bool PlayerBoard::AddShip(const Game::Ship& ship) {
        for (const auto& pos : ship.squares) {
            if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
                return false;
            }
        }

        for (const auto& existingShip : ships) {
            for (const auto& pos : existingShip.squares) {
                for (const auto& newShipPos : ship.squares) {
                    if (pos.x == newShipPos.x && pos.y == newShipPos.y) {
                        return false;
                    }
                }
            }
        }

        ships.push_back(ship);
        return true;
    }

    bool PlayerBoard::TakeShot(Game::Coords pos) {
        for (auto& ship : ships) {
            for (size_t i = 0; i < ship.squares.size(); ++i) {
                if (ship.squares[i].x == pos.x && ship.squares[i].y == pos.y) {
                    ship.hits[i] = true;
                    shots.push_back(Game::Shot(pos, true));
                    return true;
                }
            }
        }

        shots.push_back(Game::Shot(pos, false));
        return false;
    }

    bool PlayerBoard::Shoot(Game::Coords pos, PlayerBoard& other) {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
            return false;
        }

        return other.TakeShot(pos);
    }

    bool PlayerBoard::IsDead() {
        for (const auto& ship : ships) {
            for (const auto& hit : ship.hits) {
                if (!hit) {
                    return false;
                }
            }
        }
        return true;
    }
}

