#include <ext/SDL/include/SDL.h>
#include <cstdlib>
#include <iostream>

#include <window.h>
#include <renderer.h>
#include <scene.h>

int main()
{
  SDL_Event event;
  int quit = 0;

  Renderer renderer;

  Scene headScene("res/africanhead.obj");
  Scene cubeScene("res/Cube.gltf");

  while( !quit )
  {
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

  printf("closing\n");
  SDL_Quit();
}
