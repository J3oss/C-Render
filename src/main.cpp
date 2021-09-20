#include <ext/SDL/include/SDL.h>
#include <cstdlib>
#include <iostream>

#include <window.h>
#include <renderer.h>
#include <scene.h>
#include <scene_manager.h>

int main()
{
  SDL_Event event;
  int quit = 0;

  SceneManager sceneManager;

  auto headScene = new Scene("res/africanhead.obj");
  std::shared_ptr<Scene> pHeadScene(headScene);
  sceneManager.AddScene(pHeadScene);

  auto cubeScene = new Scene("res/Cube.gltf");
  std::shared_ptr<Scene> pCubeScene(cubeScene);
  sceneManager.AddScene(pCubeScene);

  Renderer renderer;

  while( !quit )
  {
    renderer.Clear();
    renderer.DrawScene(sceneManager.GetScene(0));
    renderer.Update();

    while( SDL_PollEvent( &event ) )
    {
        switch( event.type )
        {
            case SDL_QUIT:
                quit = 1;
                break;

            default:
              break;
        }
    }
  }

  SDL_Quit();
}
