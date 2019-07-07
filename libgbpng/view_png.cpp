#include"libgbpng/png.hpp"
#include<cstdio>
#include<vector>
#include<SDL.h>


using namespace gbpng;


SDL_Window*      g_window;
SDL_Renderer*  g_renderer;
SDL_Texture*    g_texture;


void
update_screen(const direct_color_image&  img) noexcept
{
  int  pitch;

  void*  ptr;

    if(SDL_LockTexture(g_texture,nullptr,&ptr,&pitch) == 0)
    {
      auto  dst = reinterpret_cast<uint8_t*>(ptr);

        for(int  y = 0;  y < img.get_height();  ++y)
        {
          SDL_memcpy(dst,img.get_row_pointer(y),4*img.get_width());

          dst += pitch;
        }


      SDL_UnlockTexture(g_texture);

      SDL_RenderClear(g_renderer);
      SDL_RenderCopy(g_renderer,g_texture,nullptr,nullptr);

      SDL_RenderPresent(g_renderer);
    }
}



int
main(int  argc, char**  argv)
{
    if(argc != 2)
    {
      return 0;
    }


  auto  path = argv[1];

  direct_color_image  img;

  try
  {
    img.read_png_from_file(path);
  }


  catch(const std::exception&  e)
  {
    printf("%s\n",e.what());

    return -1;
  }


  int  w = img.get_width() ;
  int  h = img.get_height();

  SDL_Init(SDL_INIT_VIDEO);

  g_window = SDL_CreateWindow("GAME BABY - " __DATE__,0,0,w,h,0);

    if(!g_window)
    {
      printf("cannot create window\n");
    }


  g_renderer = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
  g_texture  = SDL_CreateTexture(g_renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STREAMING,w,h);

  static SDL_Event  evt;

  update_screen(img);

    for(;;)
    {
        while(SDL_PollEvent(&evt))
        {
            switch(evt.type)
            {
          case(SDL_WINDOWEVENT):
              update_screen(img);
              break;
          case(SDL_QUIT):
              goto QUIT;
              break;
            }
        }


      SDL_Delay(20);
    }


QUIT:
  SDL_DestroyTexture(g_texture);
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);

  SDL_Quit();

  return 0;
}




