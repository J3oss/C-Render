#include <ext/SDL/include/SDL.h>
#include <cstdlib>
#include <iostream>

#include <window.h>
#include <renderer.h>

#define WIDTH 800
#define HEIGHT 800

Color red(255,0,0);
Color blue(0,0,255);
Color green(0,255,0);
Color yellow(255,255,0);
Color purple(255,0,255);
Color white(255,255,255);

Point p1(0, 0, 0);
Point p2(799, 0, 0);
Point p3(799, 160, 0);
Point p4(799, 320, 0);
Point p5(799, 480, 0);
Point p6(799, 640, 0);
Point p7(799, 799, 0);

int main()
{
  SDL_Event event;
  int quit = 0;

  Renderer renderer(WIDTH, HEIGHT);

  while( !quit )
  {
    renderer.DrawLine(p1, p2, white);
    renderer.DrawLine(p1, p3, red);
    renderer.DrawLine(p1, p4, blue);
    renderer.DrawLine(p1, p5, green);
    renderer.DrawLine(p1, p7, purple);

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
