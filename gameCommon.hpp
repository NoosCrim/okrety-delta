#pragma once

#include <vector>

namespace Game {

    struct Coords {

        int x;
        int y;
    };

    struct Shot {

        Coords pos;
        bool hit;
    };


    struct Ship {

        std::vector<Coords> squares;
        std::vector<bool> hits;


        bool Hit(Coords pos);
        bool IsDead();  //sprawdza czy statek żyje

    };

}
