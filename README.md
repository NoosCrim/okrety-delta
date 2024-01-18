# okrety-delta
 **Aby program działał poprawnie najpierw należy włączyć launcher.py**

 Żeby skompilować moduły C++ potrzeba 'sdl2' i 'sdl2_image':

 (Linux Ubuntu, Debian, etc.)
 ```sudo apt-get install libsdl2-dev libsdl2-image-dev```

 (Linux Arch, Manjaro, etc.)
 ```sudo pacman -S sdl2 sdl2_image asio```

 I kompilacja C++:
 ```g++ -Wall -fexceptions -std=c++23  -c AVE.cpp -o .objs/AVE.o```
 ```g++ -Wall -fexceptions -std=c++23  -c clientMain.cpp -o .objs/clientMain.o```
 ```g++ -Wall -fexceptions -std=c++23  -c common.cpp -o .objs/common.o```
 ```g++ -Wall -fexceptions -std=c++23  -c event.cpp -o .objs/event.o```
 ```g++ -Wall -fexceptions -std=c++23  -c gameClient.cpp -o .objs/gameClient.o```
 ```g++ -Wall -fexceptions -std=c++23  -c gameCommon.cpp -o .objs/gameCommon.o```
 ```g++ -Wall -fexceptions -std=c++23  -c gameCONSTS.cpp -o .objs/gameCONSTS.o```
 ```g++ -Wall -fexceptions -std=c++23  -c gameGUI.cpp -o .objs/gameGUI.o```
 ```g++ -Wall -fexceptions -std=c++23  -c networking/client.cpp -o .objs/networking/client.o```
 ```g++ -Wall -fexceptions -std=c++23  -c networking/messages.cpp -o .objs/networking/messages.o```
 ```g++ -Wall -fexceptions -std=c++23  -c sprite.cpp -o .objs/sprite.o```
 ```g++ -Wall -fexceptions -std=c++23  -c texture.cpp -o .objs/texture.o```
 ```g++ -Wall -fexceptions -std=c++23  -c window.cpp -o .objs/window.o```
 ```g++  -o okrety-delta-client .objs/AVE.o .objs/clientMain.o .objs/common.o .objs/event.o .objs/gameClient.o .objs/gameCommon.o .objs/gameCONSTS.o .objs/gameGUI.o .objs/networking/client.o .objs/networking/messages.o .objs/sprite.o .objs/texture.o .objs/window.o   -lSDL2 -lSDL2_image```

 lub użyć CodeBlocks i skompilować projekt ```okrety-delta.cbp```
 

 Żeby skompilować moduł Python trzeba mieć zainstalowaną paczkę 'tkinter':

 (Windows)
 ```pip install tk```

 (Linux Ubuntu, Debian, etc.)
 ```sudo apt install python3-tk```

 (Linux Arch, Manjaro, etc.)
 ```sudo pacman -S tk```

 I kompilacja Python:
 ```python launcher.py```