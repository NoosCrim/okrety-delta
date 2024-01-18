#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "AVE.hpp"
#include "gameClient.hpp"
#include "gameCommon.hpp"
#include "gameCONSTS.hpp"
#include "networking/client.hpp"
class AsyncClient;
class FlashAnim : public virtual AVE::Sprite
{
private:
    int visibleFrames, hiddenFrames, cycles, i = 0;
    void OnUpdate()
    {
        if(i/(visibleFrames+hiddenFrames) >= cycles)
        {
            delete this;
            return;
        }
        if(i%(visibleFrames+hiddenFrames) < visibleFrames)
            visible = true;
        else visible = false;
        i++;
    }
    FlashAnim(AVE::Texture* tex, int _visibleFrames, int _hiddenFrames, int _cycles, int sX = 0, int sY = 0, int sW = 0, int sH = 0, float x = 0, float y = 0, float w = 0, float h = 0, float angle = 0, bool flipH = 0, bool flipV = 1):
        AVE::Sprite(tex,sX,sY,sW,sH,x,y,w,h,angle,flipH,flipV),
        visibleFrames(_visibleFrames),
        hiddenFrames(_hiddenFrames),
        cycles(_cycles)
    {

    }
public:
    virtual ~FlashAnim()
    {

    }
    static AVE::Sprite* CreateFlashAnim(AVE::Texture* tex, int _visibleFrames, int _hiddenFrames, int _cycles, int sX = 0, int sY = 0, int sW = 0, int sH = 0, float x = 0, float y = 0, float w = 0, float h = 0, float angle = 0, bool flipH = 0, bool flipV = 0)
    {
        return new FlashAnim(tex, _visibleFrames, _hiddenFrames, _cycles, sX, sY, sW, sH, x, y, w, h, angle, flipH, flipV);
    }
};
class OkretyGame : public AVE::Window
{
    void clickHandler1();
    void clickHandler2();
    void EndTurn();
    void StartTurn();
    void WaitAnswer();
    void MessageHandler(MessageCode msgCode, int x, int y, int playerNum);
    class TimerBar : virtual AVE::Sprite
    {
    private:
        TimerBar(AVE::Texture* tex, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, unsigned int _duration);
        int SsW, SW;
        unsigned long long startingTime;
        unsigned int duration;
        void OnUpdate() override;
    public:
        static TimerBar* CreateTimerBar(AVE::Texture* tex, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, unsigned int _duration);
    };
    class Button : public AVE::Sprite, public AVE::Clickable
    {
    protected:
        virtual bool OnClick(int mX, int mY, uint8_t clicks);
        Button(AVE::Texture* _tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h, std::function<void(void)> _clickHandler);
    public:
        std::function<void(void)> clickHandler;
        static Button *CreateButton(AVE::Texture* _tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h, std::function<void(void)> _clickHandler);
    };
    class Board : public AVE::Sprite, public AVE::Clickable
    {
        const int bW, bH;
        const Sprite* marker;
        const OkretyGame &game;
        Board(AVE::Texture* tex, const AVE::Sprite* mark, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, int _bW, int _bH, const OkretyGame& _game);
        virtual bool OnClick(int mX, int mY, uint8_t clicks);
    public:
        std::vector<AVE::Sprite*> tempMarkers, permMarkers;
        std::vector<Game::Coords> marked;
        bool interactable = true, capOne = false;
        static Board* CreateBoard(AVE::Texture* tex, AVE::Sprite* mark, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, int _bW, int _bH, const OkretyGame& _game);
        virtual ~Board();
    };
private:
    unsigned int placedShips = 0;
    std::vector<Game::Coords> shots;
    float margin = 0.1f, buttonMargin = 0.1f, t;
    int wW, wH;
    AVE::Texture* digits[10];
    AVE::Texture *checkerTex, *crossTex, *checkTex, *squareTex, *circleTex, *blueSquareTex, *blueCircleTex, *progress_bar,
    *enemy_turn_header, *shoot_your_shot_header, *wait_for_answer_header, *place_ship_header, *loser, *winner;
    AVE::Sprite *marker1, *marker2, *header, *headerNum;
    TimerBar *timer = nullptr;
    Button *checkButton;
    Board *playerBoardSprite, *enemyBoardSprite;
    Game::Client::PlayerBoard playerBoard;
    AsyncClient& netClient;
    void OnStart() override;
    void OnCloseAttempt() override;
public:
    OkretyGame(AsyncClient& _netClient);
};
