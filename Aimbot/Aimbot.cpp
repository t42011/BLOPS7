#include <fstream>
#include <string>
#include <wtypes.h>
#include "../Utility/Globals.h"
#include "../Utility/Vectors.h"
#include "../Kmbox/KmboxNet.h"
#include "../Kmbox/kmbox_interface.hpp"
#include "../DMALibrary/Memory/Memory.h"
#include "../Aimbot/Aimbot.h"

bool Aimbot::init() {
    std::ifstream ipFile("kmboxnet/ip.txt");
    if (ipFile.peek() != std::ifstream::traits_type::eof()) {
        if (!ipFile.is_open()) {
            return false;
        }

        std::ifstream portFile("kmboxnet/port.txt");

        if (!portFile.is_open()) {
            return false;
        }

        std::ifstream uuidFile("kmboxnet/uuid.txt");

        if (!uuidFile.is_open()) {
            return false;
        }

        std::string uuid;
        std::string ip;
        std::string port;

        if (!std::getline(uuidFile, uuid)) return false;
        if (!std::getline(ipFile, ip)) return false;
        if (!std::getline(portFile, port)) return false;

        WORD portW = static_cast<WORD>(std::stoi(port));

        int resultNet = InitDevice(ip, portW, uuid);

       
        Globals::aimType = NET;

    }
    else {
        std::string port = find_port("USB-SERIAL CH340");
        if (port.empty()) {
            port = find_port("USB-Enhanced-SERIAL CH343");
        }
        if (!port.empty()) {

            if (!open_port(hSerial, port.c_str(), CBR_115200)) {
                return false;
            }
            else {
                Globals::aimType = BPRO;
            }
        }
        else {
            return false;
        }
    }
    return true;
}

void aimAt(FVector2D target) {
    switch (Globals:: aimType) {
    case BPRO: {
        std::string command = "km.move(" + std::to_string(target.X) + "," + std::to_string(target.Y) + ")\r\n";
        send_command(hSerial, command.c_str());
        break;
    }
    case NET:
        kmNet_mouse_move(target.X, target.Y);
        break;
    case MEMORY:
     
        break;
    }

}
 void Aimbot::aimbot(FVector2D target) {


    if (!mem.GetKeyboard()->IsKeyDown(Config.AimbotKey)) return;

    float x = target.X;
    float y = target.Y;

    FVector2D ScreenCenter((float)Globals::screenWidth / 2.0f, (float)Globals::screenHeight / 2.0f);
    FVector2D Target(0.0f, 0.0f);

    if (x != 0.0f)
    {
        if (x > ScreenCenter.X)
        {
            Target.X = -(ScreenCenter.X - x);
            if (Target.X + ScreenCenter.X > ScreenCenter.X * 2.0f) {
                Target.X = 0.0f;
            }
        }

        if (x < ScreenCenter.X)
        {
            Target.X = x - ScreenCenter.X;
            if (Target.X + ScreenCenter.X < 0.0f) {
                Target.X = 0.0f;
            }
        }
    }
    if (y != 0.0f)
    {
        if (y > ScreenCenter.Y)
        {
            Target.Y = -(ScreenCenter.Y - y);
            if (Target.Y + ScreenCenter.Y > ScreenCenter.Y * 2.0f) {
                Target.Y = 0.0f;
            }
        }

        if (y < ScreenCenter.Y)
        {
            Target.Y = y - ScreenCenter.Y;
            if (Target.Y + ScreenCenter.Y < 0.0f) {
                Target.Y = 0.0f;
            }
        }
    }

    FVector2D Angles(Target.X / (float)Globals::smoothing, Target.Y / (float)Globals::smoothing);
    aimAt(Angles);

}


   void Shoot() {
     switch (Globals::aimType) {
     case BPRO: {
         std::string cmd = "km.left(1)\r\n";
         send_command(hSerial, cmd.c_str());
         break;
     }
     case NET:
         kmNet_mouse_left(1);
         break;
     case MEMORY:

         break;
     }

 }

 static void Release(FVector2D target) {
     switch (Globals::aimType) {
     case BPRO: {
         std::string command = "km.left(0))\r\n";
         send_command(hSerial, command.c_str());
         break;
     }
     case NET:
         kmNet_mouse_left(1);
         break;
     case MEMORY:

         break;
     }

 }


 void Aimbot::Triggerbot(FVector2D target) {
     static uint64_t keyDownTime = 0;
     if (mem.GetKeyboard()->IsKeyDown(Config.TriggerbotKey)) {
         if (!keyDownTime) keyDownTime = GetTickCount64(); 
         if (GetTickCount64() - keyDownTime >= Config.TriggerDelay) {
             Shoot();
             Release(target);
         }
     }
     else {
         keyDownTime = 0;
     }
 }
 