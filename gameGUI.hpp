#pragma once
#include <iostream>
#include <vector>
#include "AVE.hpp"
class fsToggleButton : public AVE::Sprite, public AVE::Clickable
{
    int mode = 0;
protected:
    virtual bool OnClick(int mX, int mY, uint8_t clicks)
    {
        if(mX >= x && mX - x <= w && mY >= y && mY - y <= h)
        {
            if(mode == 0)
            {
                GetWindow()->SetWindowed();
                mode = (mode+1)%3;
            }
            else if(mode == 1)
            {
                GetWindow()->SetFullscreen();
                mode = (mode+1)%3;
            }
            else if(mode == 2)

            {
                GetWindow()->SetBorderless();
                mode = (mode+1)%3;
            }
            return true;
        }
        return false;
    }
    fsToggleButton(AVE::Texture* _tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h) :
        Sprite(_tex, _sX, _sY, _sW, _sH, _x, _y, _w, _h, 0, false, false),
        Clickable(_tex->GetOwner())
    {

    }
public:
    static fsToggleButton *CreateTestButton(AVE::Texture* _tex, int _sX, int _sY, int _sW, int _sH, float _x, float _y, float _w, float _h)
    {
        return new fsToggleButton(_tex, _sX, _sY, _sW, _sH, _x, _y, _w, _h);
    }
};
class board : public AVE::Sprite, public AVE::Clickable
{
    const int bW, bH;
    std::vector<std::vector<AVE::Sprite*>> markers;
    const Sprite* marker;
    board(AVE::Texture* tex, const AVE::Sprite* mark, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, int _bW, int _bH) :
        AVE::Sprite(tex,_sX,_sY,_sW,_sH, _x, _y, _w, _h, 0, false, false),
        AVE::Clickable(tex->GetOwner()),
        bW(_bW), bH(_bH),
        markers(_bW, std::vector<Sprite*>(_bH, nullptr)),
        marker(mark)
    {

    }
    virtual bool OnClick(int mX, int mY, uint8_t clicks) override
    {
        float squareW = w / bW, squareH = h / bH;
        float fX = mX - x, fY = mY - y;
        if(fX < w && fY < h && fX >= 0 && fY >= 0)
        {
            int bX = int(fX/squareW), bY = int(fY/squareH);
            //std::cout << "Clicked on: (" << bX << ", " << bY << ")" << std::endl;
            if(markers[bX][bY])
            {
                delete markers[bX][bY];
                markers[bX][bY] = nullptr;
            }
            else
            {
                markers[bX][bY] = AVE::Sprite::CreateSprite(marker);
                int mS = squareW < squareH?squareW:squareH;
                markers[bX][bY]->x = bX * squareW - mS/2 + x + squareW/2;
                markers[bX][bY]->y = bY * squareH - mS/2 + y + squareH/2;
                markers[bX][bY]->w = markers[bX][bY]->h = mS;

            }
            return true;
        }
        return false;
    }
public:
    static board* CreateBoard(AVE::Texture* tex, AVE::Sprite* mark, int _sX, int _sY, int _sW, int _sH, int _x, int _y, int _w, int _h, int _bW, int _bH)
    {
        return new board(tex,mark,_sX,_sY,_sW,_sH,_x,_y,_w,_h,_bW,_bH);
    }
};
class mWindow : public AVE::Window
{
    uint8_t I = 0;
    AVE::Texture *checkerTex, *markTex;
    AVE::Sprite *marker;
    board *bgBoard1, *bgBoard2;
    fsToggleButton *testButton;
    int _bW, _bH;
    virtual void OnStart()
    {
        checkerTex = LoadTexture("assets/checkerboard.png");
        markTex = LoadTexture("assets/crossMarker.png");
        marker = AVE::Sprite::CreateSprite(markTex,0,0,2000,2000,0,0,50,50,0,false,false);
        marker->visible = false;
        int wW, wH;
        GetSize(&wW, &wH);
        int t = (wW/2) < wH? (wW/2) : wH;
        bgBoard1 = board::CreateBoard(checkerTex,marker,0,0,_bW,_bH,t*0.1,t*0.1,t*0.85,t*0.85,_bW,_bH);
        bgBoard2 = board::CreateBoard(checkerTex,marker,0,0,_bW,_bH,t*1.05,t*0.1,t*0.85,t*0.85,_bW,_bH);
        testButton = fsToggleButton::CreateTestButton(markTex, 0,0, 32, 32, 0, 0, 200, 200);
    };
    virtual void  OnUpdate()
    {

    };
public:
    mWindow(int boardWidth, int boardHeight) : _bW(boardWidth), _bH(boardHeight)
    {

    }
};
