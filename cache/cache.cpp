#include <iostream>
#include <vector>
#include <atomic>
#include "../Utility/Vectors.h"
#include "../Utility/Globals.h"
#include "../DMALibrary/Memory/Memory.h"
#include "decrypt.h"
#include "cache.h"
#include "../Aimbot/Aimbot.h"

 

void Game::Loop(_PlayerCache& localplayer) {
    std::cout << "WAITING FOR GAME..\n" << std::endl;

    while (true) {

        INIT:

        if (IsInGame()) {


            cache.ClientInfo = decrypt_client_info();
            if (!cache.ClientInfo) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            cache.ClientBase = decrypt_client_base(cache.ClientInfo);
            if (!cache.ClientBase) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            cache.BoneBase = decrypt_bone_base();

            cache.RefDefptr = decrypt_refdef->retrieve_ref_def();

            
            localplayer.get_index(cache.ClientInfo);

            localplayer.PlayerAddress = cache.ClientBase + (localplayer.index * Globals::Offset.player_size);

            auto handle = mem.CreateScatterHandle();
            localplayer.GetCamera(handle);
            localplayer.UpdateAttributes(handle);
            mem.ExecuteReadScatter(handle);

            if (localplayer.index < 0 || localplayer.index >= 150 || !IsValidVA(localplayer.PlayerAddress)) {
                mem.CloseScatterHandle(handle);
                std::cout << "Invalid localplayer data, retrying..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            localplayer.CameraUpdate(handle);
            localplayer.updatePlayer(handle);
            mem.ExecuteReadScatter(handle);

            if (!localplayer.valid || localplayer.PosPTR == 0) {
                mem.CloseScatterHandle(handle);
                std::cout << "Localplayer not ready, retrying..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            mem.CloseScatterHandle(handle);
            std::cout << "Initialization successful! LocalPlayer index: " << localplayer.index << std::endl;

            int playercount = PlayerCount();
            int frameCount = 0;
            int playerBatch = 0;
            const int PLAYERS_PER_FRAME = 40;

            cache.Players.clear();
            cache.Players.reserve(playercount);
            for (int i = 0; i < playercount; i++) {
                if (i == localplayer.index) continue;
                _PlayerCache player;
                player.index = i;
                player.PlayerAddress = cache.ClientBase + (i * Globals::Offset.player_size);
                player.decryptedBoneIndex = get_bone_index(i);
                cache.Players.push_back(player);
            }

          //  static std::thread aimbotThread([&] { Aimbot::Loop(cache, localplayer); });
           // aimbotThread.detach();


            while (true) {

                  if (Config.refreshcheat) {
                    Config.refreshcheat = false;
                    cache.LocalPlayer;
                    cache.Players.clear();
                    cache.ClientBase = 0;
                    cache.ClientInfo = 0;
                    cache.RenderPlayerCount = 0;
                    std::cout << "refreshing..";
                    mem.FullRefresh();
                    goto INIT;
                    continue;
                }
                  auto loopStart = std::chrono::high_resolution_clock::now();

                  handle = mem.CreateScatterHandle();
                  localplayer.GetCamera(handle);
                  localplayer.UpdateAttributes(handle);

                  int batchStart = playerBatch;
                  int batchEnd = (std::min)(batchStart + PLAYERS_PER_FRAME, (int)cache.Players.size());

                  for (int i = batchStart; i < batchEnd; i++) {
                      cache.Players[i].UpdateAttributes(handle);
                  }

                  

                  if (frameCount % 3 == 0) {
                      cache.UpdateScoreboardData(handle);
                  }

                  mem.ExecuteReadScatter(handle);

                  localplayer.CameraUpdate(handle);
                  localplayer.updatePlayer(handle);

                  for (int i = batchStart; i < batchEnd; i++) {
                      if (cache.Players[i].valid) {
                          cache.Players[i].updatePlayer(handle);
                          cache.Players[i].UpdateBonePtr(handle);
                      }
                  }
                  mem.ExecuteReadScatter(handle);

                  // Queue bone reads for valid close players
                  for (int i = batchStart; i < batchEnd; i++) {
                      if (cache.Players[i].valid && cache.Players[i].PosPTR != 0 && cache.Players[i].bonePtr != 0) {
                          float dist = localplayer.pos.DistTo(cache.Players[i].pos) / 100.0f;
                          if (dist < 500.0f) {
                             // cache.Players[i].UpdateBones(handle);
                          }
                      }
                  }
                  mem.ExecuteReadScatter(handle);

                  // Process bones after scatter read completes
                  for (int i = batchStart; i < batchEnd; i++) {
                      if (cache.Players[i].valid && cache.Players[i].PosPTR != 0 && cache.Players[i].bonePtr != 0) {
                          float dist = localplayer.pos.DistTo(cache.Players[i].pos) / 100.0f;
                          if (dist < 500.0f) {
                             // cache.Players[i].ProcessBones(localplayer);
                          }
                      }
                  }

                  mem.CloseScatterHandle(handle);

                  playerBatch = (batchEnd >= cache.Players.size()) ? 0 : batchEnd;

                  bool shouldDebug = (frameCount % 120 == 0);

                  Game::PlayerLoop(shouldDebug, localplayer);
                  frameCount++;
                  
                  auto loopEnd = std::chrono::high_resolution_clock::now();
                  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(loopEnd - loopStart).count();
                  Config.readFPS = elapsed > 0 ? (1000 / elapsed) : 0;

                  

                  std::array<_PlayerCache, 150> localPlayers;
                  int count = 0;
                  {
                      std::lock_guard<std::mutex> lock(cache.cacheMutex);
                      count = cache.RenderPlayerCount;
                      for (int i = 0; i < count; ++i) localPlayers[i] = cache.RenderPlayers[i];
                  }

                  FVector2D best{};
                  int bestIndex = -1;
                  bool bestVisible = false;
                  float bestDist = FLT_MAX;

                  for (int i = 0; i < count; ++i) {
                      const auto& p = localPlayers[i];

                      if (!p.valid || p.teamId > 300 || p.index == localplayer.index || p.teamId == localplayer.teamId) continue;
                      if (Config.AimbotVisibleOnly && !p.visible) continue;
                      if (p.headW2S.IsZeroVector() || (p.status & DEAD) || p.NameEnt.health <= 0) continue;
                      if (Config.IgnoreKnocked && (p.status & KNOCKED)) continue;

                      if (p.headW2S.X < 0 || p.headW2S.Y < 0 || p.headW2S.X > Globals::screenWidth || p.headW2S.Y > Globals::screenHeight) continue;

                      float dx = Globals::screenWidth * 0.5f - p.headW2S.X;
                      float dy = Globals::screenHeight * 0.5f - p.headW2S.Y;
                      float dist = sqrtf(dx * dx + dy * dy);

                      if (dist > Config.fov)

                      if (p.distance > Config.MaxAimDistance) continue;

                      float metric = (Config.TargetSelection == 0) ? dist : p.distance;

                      if (metric < bestDist) {
                          bestDist = metric;
                          best = p.headW2S;
                          bestIndex = p.index;
                          bestVisible = p.visible;
                      }
                  }

                  if (bestIndex != -1) {
                    //  best.Y += 4.0f;

                      if (Config.EnableAimbot)
                          Aimbot::aimbot(best);

                      if (Config.EnableTrigger && bestVisible) {
                          auto& p = localPlayers[bestIndex];
                          if (p.distance <= Config.MaxTriggerDistance)
                              Aimbot::Triggerbot(best);
                      }
                  }
                  if (!IsInGame()) break;
                  std::this_thread::sleep_for(std::chrono::milliseconds(8));
            }

            std::cout << "CLEANING UP.." << std::endl;
            cache.Players.clear();
            cache.ClientBase = 0;
            cache.ClientInfo = 0;
            cache.RenderPlayerCount = 0;
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}
void Game::PlayerLoop(bool debug, _PlayerCache& localplayer) {
    static int nameUpdateCounter = 0;
    ////why even update name this much 
    if (nameUpdateCounter++ % 90 == 0) {
        auto nameHandle = mem.CreateScatterHandle();
        uint64_t nameListBase = localplayer.Retrieve_name_list();

        localplayer.QueueNameRead(nameHandle, nameListBase);

        int nameCount = 0;
        for (auto& player : cache.Players) {
            if (!player.valid || nameCount++ > 150) continue;
            player.QueueNameRead(nameHandle, nameListBase);
        }
        mem.ExecuteReadScatter(nameHandle);
        mem.CloseScatterHandle(nameHandle);
    }
    
    std::vector<std::pair<int, float>> playersByDistance;
    playersByDistance.reserve(cache.Players.size());

    int validCount = 0;
    for (size_t i = 0; i < cache.Players.size(); i++) {
        auto& player = cache.Players[i];
        if (!player.valid || player.PosPTR == 0) {
            player.headW2S = { 0, 0 };
            player.RootW2S = { 0, 0 };
            continue;
        }
        
        cache.visibilityBits = mem.Read<clientbits>(cache.ClientInfo + Globals::Offset.o_visible_bit);
        float dist = localplayer.pos.DistTo(player.pos) / 100.0f;
        player.distance = (int)dist;
        playersByDistance.push_back({ i, dist });
        player.visible = isVisible(cache.visibilityBits,player.index);
        FVector head3D;
        if (player.valid) {

            head3D = player.pos;

            if (player.stance == EEntityStance::STAND)
                head3D.Z += 55.0f;
            else if (player.stance == EEntityStance::CROUCH)
				head3D.Z += 30.0f;
            else
				head3D.Z += 20.0f;
            
            W2S(head3D, player.headW2S, localplayer.cam);
            W2S(player.pos, player.RootW2S, localplayer.cam);
            
        }
        else {
            player.headW2S = { 0, 0 };
            player.RootW2S = { 0, 0 };
        }
        
        validCount++;

    }

    std::sort(playersByDistance.begin(), playersByDistance.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    if (debug) {
        std::cout << "Valid: " << validCount << "/" << cache.Players.size()

            << "| FPS: " << Config.readFPS << std::endl;
    }

    cache.SwapBuffers(localplayer, playersByDistance);
}