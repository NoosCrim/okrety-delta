#include <iostream>
#include <vector>
#include "AVE.hpp"
#include "gameClient.hpp"
#include "gameGUI.hpp"
#include "networking/messages.hpp"
#include "gameCONSTS.hpp"
#include <ctime>
OkretyGame::TimerBar::TimerBar(AVE::Texture* tex, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, unsigned int _duration):
    AVE::Sprite(tex,_sX,_sY,_sW,_sH,_x,_y,_w,_h,0.0f,false,false),
    SsW(_sW), SW(_w),
    startingTime(time(NULL)),
    duration(_duration)
{

}
OkretyGame::TimerBar* OkretyGame::TimerBar::CreateTimerBar(AVE::Texture* tex, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, unsigned int _duration)
{
    return new TimerBar(tex,_sX,_sY,_sW,_sH,_x,_y,_w,_h,_duration);
}
void OkretyGame::TimerBar::OnUpdate()
{
    unsigned long long currTime = time(NULL);
    if(currTime - startingTime > duration)
    {
        return;
    }
    sW = SsW * (float)(duration - currTime + startingTime) / duration;
    w = SW * (float)(duration - currTime + startingTime) / duration;
}
void OkretyGame::EndTurn()
{
    AVE::Sprite *temp = header;
    header = AVE::Sprite::CreateSprite(enemy_turn_header,temp->sX,temp->sY,temp->sW,temp->sH,temp->x,temp->y,temp->w,temp->h);
    if(temp)
    delete temp;
    checkButton->visible = true;
    checkButton->visible = false;
    enemyBoardSprite->interactable = false;
}
void OkretyGame::StartTurn()
{
    AVE::Sprite *temp = header;
    header = AVE::Sprite::CreateSprite(shoot_your_shot_header,temp->sX,temp->sY,temp->sW,temp->sH,temp->x,temp->y,temp->w,temp->h);
    if(temp)
    delete temp;
    checkButton->visible = true;
    enemyBoardSprite->interactable = true;
}
void OkretyGame::WaitAnswer()
{
    AVE::Sprite *temp = header;
    header = AVE::Sprite::CreateSprite(wait_for_answer_header,temp->sX,temp->sY,temp->sW,temp->sH,temp->x,temp->y,temp->w,temp->h);
    if(temp)
    delete temp;
    checkButton->visible = true;
    checkButton->visible = false;
    enemyBoardSprite->interactable = false;
}
void OkretyGame::MessageHandler(MessageCode msgCode, int x, int y, int playerNum)
{
    LockUpdate();
    std::cout << "StartHandler" << std::endl;
    switch(msgCode)
    {
    case MessageCode::startTury:
        if(playerNum == netClient.getPlayerNumber())
        {
            if(timer) delete timer;
            timer = TimerBar::CreateTimerBar(progress_bar,0,0,16,4,0,wH-t*margin/4, wW, t*margin/4, DEFAULT_TURN_TIME);
            StartTurn();
        }
        else
        {
            if(timer) delete timer;
            timer = TimerBar::CreateTimerBar(progress_bar,0,0,16,4,0,wH-t*margin/4, wW, t*margin/4, DEFAULT_TURN_TIME);
            EndTurn();
        }
        break;
    case MessageCode::trafiony:
        if(playerNum == netClient.getPlayerNumber())
        {
            enemyBoardSprite->marked.clear();
            AVE::Sprite *tempMarker = enemyBoardSprite->tempMarkers.back();
            enemyBoardSprite->tempMarkers.clear();
            enemyBoardSprite->permMarkers.push_back(AVE::Sprite::CreateSprite(crossTex,
                tempMarker->sX,tempMarker->sY,tempMarker->sW,tempMarker->sH,
                tempMarker->x,tempMarker->y,tempMarker->w,tempMarker->h));
            shots.push_back({x,y});
            if(timer) delete timer;
            timer = nullptr;
            EndTurn();
            delete tempMarker;
        }
        break;
    case MessageCode::nieTrafiony:
        if(playerNum == netClient.getPlayerNumber())
        {
            enemyBoardSprite->marked.clear();
            AVE::Sprite *tempMarker = enemyBoardSprite->tempMarkers.back();
            enemyBoardSprite->tempMarkers.clear();
            enemyBoardSprite->permMarkers.push_back(AVE::Sprite::CreateSprite(circleTex,
                tempMarker->sX,tempMarker->sY,tempMarker->sW,tempMarker->sH,
                tempMarker->x,tempMarker->y,tempMarker->w,tempMarker->h));
            shots.push_back({x,y});
            if(timer) delete timer;
            timer = nullptr;
            EndTurn();
            delete tempMarker;
        }
        break;
    case MessageCode::niePoprawnyStrzal:
        if(playerNum == netClient.getPlayerNumber())
        {
            std::clog << "Shot deemed incorrect" << std::endl;
            FlashAnim::CreateFlashAnim(crossTex,12,24,3,0,0,32,32,enemyBoardSprite->x,enemyBoardSprite->y,enemyBoardSprite->w,enemyBoardSprite->h);
            checkButton->visible = true;
            StartTurn();
        }
        break;
    case MessageCode::strzal:
        if(playerNum != netClient.getPlayerNumber())
        {
            float squareW = playerBoardSprite->w / BATTLESHIPS_BOARD_SIZE, squareH = playerBoardSprite->h / BATTLESHIPS_BOARD_SIZE;
            if(playerBoard.TakeShot({x,y}))
            {
                playerBoardSprite->permMarkers.push_back(AVE::Sprite::CreateSprite(crossTex, 0,0,32,32, playerBoardSprite->x+squareW*x, playerBoardSprite->y+squareH*y, squareW, squareH));
                if(playerBoard.IsDead())
                    netClient.graczPrzegral(netClient.getPlayerNumber());
                else
                    netClient.trafil(x,y,playerNum);
            }
            else
            {
                playerBoardSprite->permMarkers.push_back(AVE::Sprite::CreateSprite(circleTex, 0,0,32,32, playerBoardSprite->x+squareW*x, playerBoardSprite->y+squareH*y, squareW, squareH));
                netClient.nieTrafil(x,y,playerNum);
            }
            if(timer) delete timer;
            timer = nullptr;
        }
        break;
    case MessageCode::przegranaGracza:
        if(playerBoardSprite) delete playerBoardSprite;
        if(enemyBoardSprite) delete enemyBoardSprite;
        playerBoardSprite = enemyBoardSprite = nullptr;
        if(header)delete header;
        if(headerNum) delete headerNum;
        if(checkButton) delete checkButton;
        if(timer) delete timer;
        if(playerNum == netClient.getPlayerNumber())
        {
            if(wW / 256 > wH / 64)
                header = AVE::Sprite::CreateSprite(loser,0,0,256,64,wW/2-wH*256/64*(1.0f-margin)/2,wH/2-wH*(1.0f-margin)/2,wH*256/64*(1.0f-margin),wH*(1.0f-margin));
            else
                header = AVE::Sprite::CreateSprite(loser,0,0,256,64,wW/2-wW*(1.0f-margin)/2,wH/2-wW*64/256*(1.0f-margin)/2,wW*(1.0f-margin),wW*64/256*(1.0f-margin));
        }
        else
        {
            if(wW / 256 > wH / 64)
                header = AVE::Sprite::CreateSprite(winner,0,0,256,64,wW/2-wH*256/64*(1.0f-margin)/2,wH/2-wH*(1.0f-margin)/2,wH*256/64*(1.0f-margin),wH*(1.0f-margin));
            else
                header = AVE::Sprite::CreateSprite(winner,0,0,256,64,wW/2-wW*(1.0f-margin)/2,wH/2-wW*64/256*(1.0f-margin)/2,wW*(1.0f-margin),wW*64/256*(1.0f-margin));
        }
        break;
    case MessageCode::wyszedlGracz:
        if(playerBoardSprite) delete playerBoardSprite;
        if(enemyBoardSprite) delete enemyBoardSprite;
        playerBoardSprite = enemyBoardSprite = nullptr;
        if(header)delete header;
        if(headerNum) delete headerNum;
        if(checkButton) delete checkButton;
        if(timer) delete timer;
        if(wW / 256 > wH / 64)
                header = AVE::Sprite::CreateSprite(winner,0,0,256,64,wW/2-wH*256/64*(1.0f-margin)/2,wH/2-wH*(1.0f-margin)/2,wH*256/64*(1.0f-margin),wH*(1.0f-margin));
            else
                header = AVE::Sprite::CreateSprite(winner,0,0,256,64,wW/2-wW*(1.0f-margin)/2,wH/2-wW*64/256*(1.0f-margin)/2,wW*(1.0f-margin),wW*64/256*(1.0f-margin));
    case MessageCode::ustawStatki:
        if(playerNum == netClient.getPlayerNumber())
            StartGame();
        break;
    default:
        std::clog << "Got message not yet handled" << std::endl;
        break;
    }
    UnlockUpdate();
    std::cout << "EndHandler" << std::endl;
}
void OkretyGame::clickHandler1()
{
    //std::cerr << playerBoardSprite->marked.size() << std::endl;
    if(playerBoardSprite->marked.size() == shipOrder[placedShips])
    {
        Game::Ship newShip;
        newShip.squares = playerBoardSprite->marked;
        newShip.hits.resize(newShip.squares.size());
        if(playerBoard.AddShip(newShip))
        {
            placedShips++;
            for(AVE::Sprite* marker : playerBoardSprite->tempMarkers)
            {
                playerBoardSprite->permMarkers.push_back(AVE::Sprite::CreateSprite(squareTex,marker->sX,marker->sY,marker->sW,marker->sH,marker->x, marker->y, marker->w, marker->h));
                delete marker;
            }
            playerBoardSprite->marked.clear();
            playerBoardSprite->tempMarkers.clear();
            if(placedShips == shipsNumber)
            {
                checkButton->clickHandler = [this](){clickHandler2();};
                playerBoardSprite->interactable = false;
                enemyBoardSprite->visible = true;
                checkButton->clickHandler = [this](){clickHandler2();};
                checkButton->visible = false;
                delete headerNum;
                headerNum = nullptr;
                WaitAnswer();
                netClient.ustawStatki(netClient.getPlayerNumber());
                if(timer) delete timer;
                timer = nullptr;
                /////////////////////////////
                //MessageHandler(MessageCode::startTury,0,0,netClient.getPlayerNumber()+1);
            }
            else
            {
                AVE::Sprite *newNum = AVE::Sprite::CreateSprite(digits[shipOrder[placedShips]], headerNum->sX, headerNum->sY, headerNum->sW, headerNum->sH,
                headerNum->x, headerNum->y, headerNum->w, headerNum->h);
                delete headerNum;
                headerNum = newNum;
            }
            return;
        }
    }
    int wW, wH;
    GetSize(&wW, &wH);
    FlashAnim::CreateFlashAnim(crossTex,12,24,3,0,0,32,32,playerBoardSprite->x,playerBoardSprite->y,playerBoardSprite->w,playerBoardSprite->h);
}
void OkretyGame::clickHandler2()
{
    if(enemyBoardSprite->tempMarkers.size() != 1)
    {
        FlashAnim::CreateFlashAnim(crossTex,12,24,3,0,0,32,32,enemyBoardSprite->x,enemyBoardSprite->y,enemyBoardSprite->w,enemyBoardSprite->h);
        return;
    }
    for(Game::Coords shot : shots)
        if(shot.x == enemyBoardSprite->marked.back().x && shot.y == enemyBoardSprite->marked.back().y)
        {
            FlashAnim::CreateFlashAnim(crossTex,12,24,3,0,0,32,32,enemyBoardSprite->x,enemyBoardSprite->y,enemyBoardSprite->w,enemyBoardSprite->h);
            return;
        }
    netClient.fire(enemyBoardSprite->marked.back().x, enemyBoardSprite->marked.back().y);
    WaitAnswer();
    //MessageHandler(MessageCode::trafiony, enemyBoardSprite->marked.back().x, enemyBoardSprite->marked.back().y, netClient.getPlayerNumber());
    //StartTurn();
}
bool OkretyGame::Button::OnClick(int mX, int mY, uint8_t clicks)
{
    if(mX >= x && mX - x <= w && mY >= y && mY - y <= h && visible)
    {
        clickHandler();
        return true;
    }
    return false;
}
OkretyGame::Button::Button(AVE::Texture* _tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h, std::function<void(void)> _clickHandler) :
    Sprite(_tex, _sX, _sY, _sW, _sH, _x, _y, _w, _h, 0, false, false),
    Clickable(_tex->GetOwner()),
    clickHandler(_clickHandler)
{

}
void (*clickHandler)();
OkretyGame::Button *OkretyGame::Button::CreateButton(AVE::Texture* _tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h, std::function<void(void)> _clickHandler)
{
    return new Button(_tex, _sX, _sY, _sW, _sH, _x, _y, _w, _h, _clickHandler);
}
OkretyGame::Board::Board(AVE::Texture* tex, const AVE::Sprite* mark, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, int _bW, int _bH, const OkretyGame &_game) :
    AVE::Sprite(tex,_sX,_sY,_sW,_sH, _x, _y, _w, _h, 0, false, false),
    AVE::Clickable(tex->GetOwner()),
    bW(_bW), bH(_bH),
    marker(mark),
    game(_game)
{

}
bool OkretyGame::Board::OnClick(int mX, int mY, uint8_t clicks)
{

    float squareW = w / bW, squareH = h / bH;
    float fX = mX - x, fY = mY - y;
    if(fX < w && fY < h && fX >= 0 && fY >= 0)
    {
        if(!interactable) return true;
        int bX = int(fX/squareW), bY = int(fY/squareH);
        //std::cout << "Clicked on: (" << bX << ", " << bY << ")" << std::endl;
        auto i = tempMarkers.begin();
        auto j = marked.begin();
        for(; i != tempMarkers.end(); i++, j++)
        {
            if(j->x == bX && j->y == bY)
            {
                delete *i;
                tempMarkers.erase(i);
                marked.erase(j);
                return true;
            }
        }
        if(capOne && !marked.empty())
        {
            marked.clear();
            delete tempMarkers.back();
            tempMarkers.clear();
        }
        tempMarkers.push_back(AVE::Sprite::CreateSprite(marker));
        int mS = squareW < squareH?squareW:squareH;
        tempMarkers.back()->x = bX * squareW - mS/2 + x + squareW/2;
        tempMarkers.back()->y = bY * squareH - mS/2 + y + squareH/2;
        tempMarkers.back()->w = tempMarkers.back()->h = mS;
        marked.push_back({bX, bY});

        return true;
    }
    return false;
}
OkretyGame::Board* OkretyGame::Board::CreateBoard(AVE::Texture* tex, AVE::Sprite* mark, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, int _bW, int _bH, const OkretyGame &_game)
{
    return new Board(tex,mark,_sX,_sY,_sW,_sH,_x,_y,_w,_h,_bW,_bH,_game);
}
OkretyGame::Board::~Board()
{
    for(AVE::Sprite *marker : tempMarkers)
        delete marker;
    for(AVE::Sprite *marker : permMarkers)
        delete marker;
}
OkretyGame::OkretyGame(AsyncClient& _netClient) :
    playerBoard(BATTLESHIPS_BOARD_SIZE, BATTLESHIPS_BOARD_SIZE),
    netClient(_netClient)
{

}
void OkretyGame::StartGame()
{
    marker1 = AVE::Sprite::CreateSprite(blueSquareTex,0,0,2000,2000,0,0,50,50,0,false,false);
    marker1->visible = false;
    marker2 = AVE::Sprite::CreateSprite(blueCircleTex,0,0,2000,2000,0,0,50,50,0,false,false);
    marker2->visible = false;
    if(header) delete header;
    float checkButtonSize, boardSize;
    t = 0.5f * wW < 0.8f * wH? 0.5f * wW : 0.8f * wH;
    boardSize = t * (1.0f-margin);
    checkButtonSize = (wW - 2*boardSize)*(1.0f-buttonMargin)/2;
    playerBoardSprite = Board::CreateBoard(checkerTex,marker1,0,0,BATTLESHIPS_BOARD_SIZE,BATTLESHIPS_BOARD_SIZE,
        margin * 0.5f * t,wH - margin*0.5f*t - boardSize,boardSize,boardSize,
        BATTLESHIPS_BOARD_SIZE,BATTLESHIPS_BOARD_SIZE, *this);
    enemyBoardSprite = Board::CreateBoard(checkerTex,marker2,0,0,BATTLESHIPS_BOARD_SIZE,BATTLESHIPS_BOARD_SIZE,
        wW - margin * 0.5f * t - boardSize, wH - margin*0.5f*t - boardSize,boardSize,boardSize,
        BATTLESHIPS_BOARD_SIZE,BATTLESHIPS_BOARD_SIZE, *this);
    playerBoardSprite -> capOne = false;
    enemyBoardSprite -> capOne = true;
    checkButton = Button::CreateButton(checkTex,0,0,32,32,wW/2 - checkButtonSize/2, wH-t/2-checkButtonSize/2, checkButtonSize, checkButtonSize, [this](){clickHandler1();});
    header = AVE::Sprite::CreateSprite(place_ship_header,0,0,128,16,t*margin*0.5f, t*margin*0.5f, (wW - t*margin) * 0.5f, (wW - t*margin) * 0.5f*0.125f);
    headerNum = AVE::Sprite::CreateSprite(digits[shipOrder[0]],0,0,16,16,t*margin*0.5f+(wW - t*margin) * 0.5f*0.333f, t*margin*0.5f, (wW - t*margin) * 0.5f*0.125f, (wW - t*margin) * 0.5f*0.125f);
    enemyBoardSprite->interactable = false;
    enemyBoardSprite->visible = false;
    //↓ will be used to create ship placing timer when needed.                                 ↓ seconds here
    timer = TimerBar::CreateTimerBar(progress_bar,0,0,16,4,0,wH-t*margin/4, wW, t*margin/4, DEFAULT_SHIP_PLACEMENT_TIME);
}
void OkretyGame::OnStart()
{
    GetSize(&wW, &wH);
    checkerTex = LoadTexture("assets/checkerboard.png");
    crossTex = LoadTexture("assets/crossMarker.png");
    squareTex = LoadTexture("assets/squareMarker.png");
    checkTex = LoadTexture("assets/checkMarker.png");
    circleTex = LoadTexture("assets/circleMarker.png");
    blueSquareTex = LoadTexture("assets/blueSquareMarker.png");
    blueCircleTex = LoadTexture("assets/blueCircleMarker.png");
    progress_bar = LoadTexture("assets/progress_bar.png");
    digits[0] = LoadTexture("assets/dig0.png");
    digits[1] = LoadTexture("assets/dig1.png");
    digits[2] = LoadTexture("assets/dig2.png");
    digits[3] = LoadTexture("assets/dig3.png");
    digits[4] = LoadTexture("assets/dig4.png");
    digits[5] = LoadTexture("assets/dig5.png");
    digits[6] = LoadTexture("assets/dig6.png");
    digits[7] = LoadTexture("assets/dig7.png");
    digits[8] = LoadTexture("assets/dig8.png");
    digits[9] = LoadTexture("assets/dig9.png");
    place_ship_header = LoadTexture("assets/place_ship.png");
    shoot_your_shot_header = LoadTexture("assets/shoot_your_shot.png");
    wait_for_answer_header = LoadTexture("assets/wait_for_answer.png");
    enemy_turn_header = LoadTexture("assets/enemy_turn.png");
    winner = LoadTexture("assets/winner.png");
    loser = LoadTexture("assets/loser.png");
    waiting_for_connection = LoadTexture("assets/waiting_for_connection.png");
    if(wW / 256 > wH / 64)
        header = AVE::Sprite::CreateSprite(waiting_for_connection,0,0,256,64,wW/2-wH*256/64*(1.0f-margin)/2,wH/2-wH*(1.0f-margin)/2,wH*256/64*(1.0f-margin),wH*(1.0f-margin));
    else
        header = AVE::Sprite::CreateSprite(waiting_for_connection,0,0,256,64,wW/2-wW*(1.0f-margin)/2,wH/2-wW*64/256*(1.0f-margin)/2,wW*(1.0f-margin),wW*64/256*(1.0f-margin));
    networking = std::thread([this](){netClient.run();});
    ///////////////////////////////////////////////
    //MessageHandler(MessageCode::ustawStatki, 0,0,netClient.getPlayerNumber());
}
void OkretyGame::OnCloseAttempt()
{
    if(playerBoardSprite)
        delete playerBoardSprite;
    if(enemyBoardSprite)
        delete enemyBoardSprite;
    netClient.wylacz();
    networking.join();
    Close();
}


