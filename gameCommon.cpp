#include "gameCommon.hpp"

namespace Game {

    bool Ship::Hit(Coords pos) {

        for (size_t i = 0; i < squares.size(); i++) {

            if (squares[i].x == pos.x && squares[i].y == pos.y) {

                hits[i] = true;
                return true;
            }
        }

        return false;
    }

}
