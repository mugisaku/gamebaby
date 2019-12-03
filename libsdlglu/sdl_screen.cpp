#include"sdl.hpp"
#include<SDL.h>
#include"libgbstd/file_op.hpp"
#include"libgbstd/ilc.hpp"
#include"libgbpng/png.hpp"



namespace sdl{


namespace{


SDL_Window*      g_window;
SDL_Surface*    g_surface;

bool  g_recording_flag;

gbpng::animation_builder
g_apng_builder;


uint32_t
g_color_table[0b1000000000];


template<typename  T>
void
transfer() noexcept
{
  const gbstd::canvas&  cv = gbstd::get_video_canvas();

  auto  p_base = static_cast<uint8_t*>(g_surface->pixels);

  int  pitch = g_surface->pitch;

  auto  w = g_surface->w;
  auto  h = g_surface->h;

    for(int  y = 0;  y < h;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

      auto  src = cv.get_pixel_pointer(0,y);

        for(int  x = 0;  x < w;  ++x)
        {
          *reinterpret_cast<T*>(p) = g_color_table[src++->color.code&0x7FFF];

          p += sizeof(T);
        }
    }
}


}




void
update_screen() noexcept
{
    if(SDL_LockSurface(g_surface) == 0)
    {
        switch(g_surface->format->BytesPerPixel)
        {
      case(1): transfer<uint8_t >();break;
      case(2): transfer<uint16_t>();break;
      case(3): transfer<uint32_t>();break;
      case(4): transfer<uint32_t>();break;
        }


      SDL_UpdateWindowSurface(g_window);

      SDL_UnlockSurface(g_surface);

      gbstd::unset_redraw_flag();
    }


    if(g_recording_flag)
    {
      auto  w = g_surface->w;
      auto  h = g_surface->h;

      gbpng::direct_color_image  img(w,h);

      auto  dst = img.get_row_pointer(0);
      auto  src = gbstd::get_video_canvas().get_pixel_pointer(0,0);

        for(int  y = 0;  y < h;  ++y)
        {
            for(int  x = 0;  x < w;  ++x)
            {
              auto  color = (*src++).color;

                if(color)
                {
                  *dst++ = color.get_r255();
                  *dst++ = color.get_g255();
                  *dst++ = color.get_b255();
                  *dst++ = 255;
                }

              else
                {
                  *dst++ =   0;
                  *dst++ =   0;
                  *dst++ =   0;
                  *dst++ = 255;
                }
            }
        }


      g_apng_builder.append(img);
    }
}


void
quit() noexcept
{
  end_screen_recording();

  quit_sound();

    if(test_sound_recording())
    {
      auto  bin = get_sound_wave_binary();

      gbstd::write_to_file(bin.data(),bin.size(),"__mksnd.wav");
    }


  SDL_DestroyWindow(g_window);

  SDL_Quit();

  fflush(stdout);

  std::_Exit(0);
}


void
init(int  w, int  h, double  scale) noexcept
{
  SDL_Init(SDL_INIT_VIDEO);

  g_window = SDL_CreateWindow("GAME BABY - " __DATE__,SDL_WINDOWPOS_CENTERED,
                                                      SDL_WINDOWPOS_CENTERED,
                                                      w,h,0);

    if(!g_window)
    {
      printf("cannot create window\n");
    }


  g_surface  = SDL_GetWindowSurface(g_window);

    for(int  b = 0;  b < 8;  ++b){
    for(int  g = 0;  g < 8;  ++g){
    for(int  r = 0;  r < 8;  ++r){
      gbstd::color   color(r,g,b);

      g_color_table[color.code&0x7FFF] = SDL_MapRGB(g_surface->format,color.get_r255(),color.get_g255(),color.get_b255());
    }}}


  gbstd::set_video_size(w,h);
}


void
init(int  x, int  y, int  w, int  h, double  scale) noexcept
{
  init(w,h,scale);

  SDL_SetWindowPosition(g_window,x,y);
}


void
resize_screen(int  w, int  h) noexcept
{
    if(!g_window)
    {
      init(w,h);
    }

  else
    {
      SDL_SetWindowSize(g_window,w,h);
    }
}


void
screenshot() noexcept
{
#ifndef __EMSCRIPTEN__
  auto  png = gbstd::get_video_image().make_png_stream();

  gbstd::write_to_file(png.data(),png.size(),"__screenshot.png");
#endif
}


void
start_screen_recording() noexcept
{
    if(!g_recording_flag)
    {
      auto  w = g_surface->w;
      auto  h = g_surface->h;

      gbpng::image_header  ihdr(w,h);

      g_apng_builder.reset(ihdr,80);

      g_recording_flag = true;

      printf("[start screen recording]\n");
    }
}


void
end_screen_recording() noexcept
{
    if(g_recording_flag)
    {
      g_recording_flag = false;

      printf("[end screen recording]\n");

      auto  chkls = g_apng_builder.build();

      chkls.write_png_to_file("__scrrec.apng");
    }
}


bool
test_screen_recording() noexcept
{
  return g_recording_flag;
}


}




