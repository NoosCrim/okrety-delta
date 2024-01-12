#pragma once

#include<vector>
#include "gameCommon.hpp"   //wszystkie potrzebne klasy(używane tu a nie są tu tworzone) są w gameCommon.hpp

namespace Game{
namespace Client{

class PlayerBoard   ///klasa
{

    public:
    //pola
        const int width;    //stała przechowująca szerokość planszy
        const int height;   //stała przechowująca wysokość planszy
        std::vector<Ship> ships; //vector obiektów klasy Ship
        std::vector<Shot> shots; //vector obiektów klasy Shot

    //konstruktor
        PlayerBoard(int width, int height);

    //metody
        bool AddShip(const Ship& ship);
        bool TakeShot(Coords pos);
};


class EnemyBoard    ///klasa abstrakcyjna
{
    public:
    //pola
        const int width;    //stała przechowująca szerokość planszy
        const int height;   //stała przechowująca wysokość planszy
        std::vector<Shot> shots; //vector obiektów klasy Shot

    //konstruktor
        EnemyBoard(int width, int height);

    //destruktor wirtualny
        virtual ~EnemyBoard(){};

    //metody
        virtual bool OnShot(Coords pos) = 0;    //Metoda abstrakcyjna
        bool Shoot(Coords pos);
};



}}
