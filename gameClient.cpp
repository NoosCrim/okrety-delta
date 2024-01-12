#include "gameClient.hpp"

namespace Game{
namespace Client{


///konstruktor PlayerBoard
    PlayerBoard::PlayerBoard(int _width, int _height) : width(_width), height(_height)
    {}

//metody PlayerBoard
    bool PlayerBoard::AddShip(const Ship &ship)
    {
        for(unsigned int i=0;i<ship.squares.size();i++)  //pętla po wszytskich elementach statku
        {
            if(ship.squares[i].x >= width || ship.squares[i].x < 0  ||  ship.squares[i].y >= height || ship.squares[i].y < 0)  //Jeżeli któryś z elementów statku nie mieści się na planszy
                return false;   //zwróc false


        }
        for(unsigned int j=0;j<ships.size();j++) //pętla po wszytskich "dobych" statkach
        {
            for(unsigned int k=0;k<ships[j].squares.size();k++)  //pętla po wszytskich elementach "dobrego" statku
            {
                for(unsigned int i=0;i<ship.squares.size();i++)
                {
                    if(ship.squares[i].x == ship.squares[j].x || ship.squares[i].y == ship.squares[j].y)    //Jeżeli koliduje z innymi "dobrymi" statkami
                    {
                        return false;   //zwróc false
                    }
                }
            }
        }

    //w przeciwnym przypadku
        ships.push_back(ship);  //dodaje go do "dobrych" statków
        return true;   //zwraca true
    }

    bool PlayerBoard::TakeShot(Coords pos)
    {
        for(unsigned int i=0;i<ships.size();i++)
        {
            if(ships[i].Hit(pos) == true)    //Wywołuje Hit(pos) na każdym statku z ships.
            {
                return true;    //jeżeli Hit zwróci true, zwróć true i zakończ pętle.
            }
        }
        //Jeżeli żaden Hit nie zwróci true to zwróć false.
            return false;

    }



///konstruktor EnemyBoard
    EnemyBoard::EnemyBoard(int _width, int _height) : width(_width), height(_height)
    {}

//metody EnemyBoard

    bool EnemyBoard::Shoot(Coords pos)
    {
        if(pos.x >= width || pos.x < 0  ||  pos.y >= height || pos.y < 0) //Jeżeli pos jest poza planszą
        {
            return false;   //zwróć false
        }
        else    //w przeciwnym wypadku wywołaj metodę OnShot(pos)
        {
            //zapisz strzał(pos i hit) czy trafił w shots
                Shot a;
                a.pos=pos;
                a.hit=OnShot(pos);
                shots.push_back(a);

                return true;
        }
    }




}}



