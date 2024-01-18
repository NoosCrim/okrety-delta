#include "gameCommon.hpp"

namespace Game {

    bool Ship::Hit(Coords pos) {

        for (std::size_t i = 0; i < squares.size(); i++) {

            if (squares[i].x == pos.x && squares[i].y == pos.y) {

                hits[i] = true;
                return true;
            }
        }

        return false;
    }


    bool Ship::IsDead() {   //sprawdza czy statek żyje

        for(bool hit : hits) {

            if(hit == false)        //jesli jakis element nie jest zestrzelony
                return false;       //to statek dalej żyje
        }

            return true;            //w przeciwnym wypadku statek nie żyje
    }

}
