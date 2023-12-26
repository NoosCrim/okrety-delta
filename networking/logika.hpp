#include <string>
#define arraySize 100
#define playerHealth 10

using std::string;

bool T[arraySize][arraySize];
short P[100];
bool czyWolne(int x, int y)
{
    return T[x][y];
}

void wyczysc()
{
    for(int i = 0; i < arraySize; i++)
    {
        for(int j = 0; j < arraySize; j++)
        {
            T[i][j] = false;
        }
    }
}

void ustaw(int x, int y)
{
    T[x][y] = true;
}

int playerNumber(string msg)
{
    return msg[0] -'0';
}

int xValue(string msg)
{
    string X = "";
    int i = 2;
    while(msg[i] != ' ') {X += msg[i]; i++;}
    return stoi(X);
}

int yValue(string msg)
{
    string Y = "";
    int i = 2;
    while(msg[i] != ' ') {i++;} i++;
    for(i;i < msg.length(); i++) Y += msg[i];
    return stoi(Y);
}

bool isMessageValid(string msg)
{
    if(msg.length() < 5) return false;
    int Player = playerNumber(msg);
    if(Player > 8 && Player < 1) return false;
    int x = xValue(msg);
    int y = yValue(msg);
}


