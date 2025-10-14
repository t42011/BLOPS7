#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include "cache/decrypt.h"
#include "cache/cache.h"
#include "Aimbot/Aimbot.h"
#include "Overlay/Render.h"


void refreshMemory()
{
    while (true)
    {
        mem.MemoryPartialRefresh();
        mem.TLBRPartialefresh();

        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}


int main()
{


    if(!mem.Init(Globals::GameTitle)) {
		Sleep(4000);
        std::cout << "Failed to initialize DMA. Exiting...\n";
        return 1;
	}

    if (!mem.GetKeyboard()->InitKeyboard())
    {
        std::cout << "Failed to initialize keyboard hotkeys through kernel." << std::endl;
        
    }


    if (!Aimbot::init())
    {
        std::cout << "Failed to initialize Aimbot. Exiting...\n";
		 
   }

	  

    mem.FullRefresh();

    std::thread(refreshMemory).detach();



   _PlayerCache localplayer;

   std::thread GameThread(Game::Loop, std::ref(localplayer));

    Render render = Render();

    render.Init();

    render.Loop();

   GameThread.join();

    return 0;
}

