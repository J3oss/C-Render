#include <ext/SDL/include/SDL.h>
#include <cstdlib>
#include <iostream>

#include <window.h>
#include <renderer.h>
#include <scene.h>
#include <scene_manager.h>

#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>

#include <var.h>

using namespace std::chrono;
float bench = 0;
uint32_t runs = 1000;
int main()
{
  SDL_Event event;
  int quit = 0;

  SceneManager sceneManager;

  auto tableScene = new Scene("res/Table/Table.gltf");
  std::shared_ptr<Scene> pTableScene(tableScene);
  sceneManager.AddScene(pTableScene);

  auto headwithcameraScene = new Scene("res/Head/Head.gltf");
  std::shared_ptr<Scene> pheadwithcameraScene(headwithcameraScene);
  sceneManager.AddScene(pheadwithcameraScene);

  // auto damagedHelmetScene = new Scene("res/DamagedHelmet/DamagedHelmet.gltf");
  // std::shared_ptr<Scene> pDamagedHelmetScene(damagedHelmetScene);
  // sceneManager.AddScene(pDamagedHelmetScene);

  // auto CesiumMilkTruckScene = new Scene("res/CesiumMilkTruck/CesiumMilkTruck.gltf");
  // std::shared_ptr<Scene> pCesiumMilkTruckScene(CesiumMilkTruckScene);
  // sceneManager.AddScene(pCesiumMilkTruckScene);

  // auto cubeplaneScene = new Scene("res/Cube&Plane/Cube&Plane.gltf");
  // std::shared_ptr<Scene> pCubePlane(cubeplaneScene);
  // sceneManager.AddScene(pCubePlane);

  Renderer renderer;
  renderer.SetViewPort(0, 0);

  FocusType focusType = FocusType::SCENES;
  std::shared_ptr<Node> focusObject = nullptr;
  std::shared_ptr<Scene>activeScene = sceneManager.GetActiveScene();

  std::cout << "Use arrow keys to move around." << '\n';
  std::cout << "Use S, C, M and L to switch the controls around on scenes, cameras, meshes and lights" << '\n';
  std::cout << "Use B and N for back and next on the controled object type." << "\n\n";

  while( !quit )
  // while (runs--)
  {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    renderer.Clear();
    renderer.DrawScene(activeScene);
    renderer.Update();

    while( SDL_PollEvent( &event ) )
    {
        switch( event.type )
        {
          case SDL_KEYUP:
          {
            switch( event.key.keysym.sym )
            {
              case SDLK_UP:
                if (!focusObject) break;
                focusObject->Translate(glm::vec3(0.0,0.0,-1.0));
                break;
              case SDLK_DOWN:
                if (!focusObject) break;
                focusObject->Translate(glm::vec3(0.0,0.0,1.0));
                break;
              case SDLK_LEFT:
                if (!focusObject) break;
                focusObject->Translate(glm::vec3(-1.0,0.0,0.0));
                break;
              case SDLK_RIGHT:
                if (!focusObject) break;
                focusObject->Translate(glm::vec3(1.0,0.0,0.0));
                break;

              case SDLK_s:
                focusType = FocusType::SCENES;
                focusObject = nullptr;
                break;
              case SDLK_c:
                focusType = FocusType::CAMERAS;
                focusObject = sceneManager.GetFocusedObject(focusType);
                break;
              case SDLK_m:
                focusType = FocusType::MESHES;
                focusObject = sceneManager.GetFocusedObject(focusType);
                break;
              case SDLK_l:
                focusType = FocusType::LIGHTS;
                focusObject = sceneManager.GetFocusedObject(focusType);
                break;

              case SDLK_q:
                quit = 1;
                break;
              case SDLK_n:
                switch (focusType)
                {
                  case FocusType::SCENES:
                  activeScene = sceneManager.NextActiveScene();
                  focusObject = nullptr;
                  break;
                  case FocusType::CAMERAS:
                  focusObject = sceneManager.NextFocusedObject(focusType);
                  break;
                  case FocusType::MESHES:
                  focusObject = sceneManager.NextFocusedObject(focusType);
                  break;
                  case FocusType::LIGHTS:
                  focusObject = sceneManager.NextFocusedObject(focusType);
                  break;
                }
                break;
            }
            break;
          }

          case SDL_QUIT:
          {
            quit = 1;
            break;
          }

          case SDL_MOUSEMOTION:
          {
            if (!focusObject) break;

            float radianX = (float)event.motion.xrel/WIDTH  * 2;
            float radianY = (float)event.motion.yrel/HEIGHT * 2;
            focusObject->Rotate(glm::vec3(radianY,radianX,0.0));
            break;
          }

          default:
            break;
        }
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    int sleepTime = (1.0f/60.0f - time_span.count()) * 1000;
    if(sleepTime > 0) std::this_thread::sleep_for(std::chrono::milliseconds( sleepTime ));

    // std::cout << "FPS(before limiting):"<< 1.0/time_span.count() << std::endl;
    // bench+=1.0/time_span.count();
  }

  // printf("avg fps: %f\n", bench/1000);
  SDL_Quit();
}
