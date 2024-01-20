# okrety-delta

 Żeby skompilować moduły C++ potrzeba 'sdl2' i 'sdl2_image':

 (Linux Ubuntu, Debian, etc.)
 ```sudo apt-get install libsdl2-dev libsdl2-image-dev```

 (Linux Arch, Manjaro, etc.)
 ```sudo pacman -S sdl2 sdl2_image asio```

 I kompilacja C++:
 
 Client:
 
``` 
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/AVE.cpp -o .objs/AVE.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/clientMain.cpp -o .objs/clientMain.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/common.cpp -o .objs/common.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/event.cpp -o .objs/event.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/gameClient.cpp -o .objs/gameClient.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/gameCommon.cpp -o .objs/gameCommon.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/gameCONSTS.cpp -o .objs/gameCONSTS.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/gameGUI.cpp -o .objs/gameGUI.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/networking/client.cpp -o .objs/networking/client.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/networking/messages.cpp -o .objs/networking/messages.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/sprite.cpp -o .objs/sprite.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/texture.cpp -o .objs/texture.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/window.cpp -o .objs/window.o
g++  -o okrety-delta-client .objs/AVE.o .objs/clientMain.o .objs/common.o .objs/event.o .objs/gameClient.o .objs/gameCommon.o .objs/gameCONSTS.o .objs/gameGUI.o .objs/networking/client.o .objs/networking/messages.o .objs/sprite.o .objs/texture.o .objs/window.o   -lSDL2 -lSDL2_image
```

Server:

```
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/common.cpp -o .objs/common.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/gameCommon.cpp -o .objs/gameCommon.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/gameCONSTS.cpp -o .objs/gameCONSTS.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/networking/client.cpp -o .objs/networking/client.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/networking/messages.cpp -o .objs/networking/messages.o
g++ -Wall -fexceptions -std=c++23  -c /home/crimsoon/code/projects/okrety-delta/serverMain.cpp -o .objs/serverMain.o
g++  -o okrety-delta-server .objs/common.o .objs/gameCommon.o .objs/gameCONSTS.o .objs/networking/client.o .objs/networking/messages.o .objs/serverMain.o  
```

 lub użyć CodeBlocks i skompilować odpowiednie build targety w projekcie ```okrety-delta.cbp```
 

 Żeby skompilować moduł Python trzeba mieć zainstalowaną paczkę 'tkinter':

 (Windows)
 
 ```pip install tk```

 (Linux Ubuntu, Debian, etc.)
 
 ```sudo apt install python3-tk```

 (Linux Arch, Manjaro, etc.)
 
 ```sudo pacman -S tk```

 I kompilacja Python:
 
 ```python [repository path]/launcher.py```
 
 
W celu włączenia serwera trzeba z terminala użyć komendy:

Linuks:

```[repository path]/okrety-delta-server```

Windows:

```[repository path]/okrety-delta-server.exe```


Natomiast w celu włączenia klienta:

 ```python [repository path]/launcher.py```


 
 
 
 
 
 
