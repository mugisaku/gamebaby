#include"sdl.hpp"
#include<SDL.h>
#include"libgbstd/file_op.hpp"
#include"libgbpng/png.hpp"



namespace sdl{


namespace{


SDL_Window*      g_window;
SDL_Renderer*  g_renderer;
SDL_Texture*    g_texture;

bool  g_recording_flag;

gbpng::animation_builder
g_apng_builder;


int  g_width;
int  g_height;

gbstd::image
g_image;

SDL_Rect  g_dst_rect;


void
transfer(const gbstd::canvas&  cv, uint8_t*  p_base, int  pitch) noexcept
{
    for(int  y = 0;  y < g_height;  ++y)
    {
      auto  p = reinterpret_cast<uint16_t*>(p_base)        ;
                                            p_base += pitch;

      auto  src = cv.get_pixel_pointer(0,y);

        for(int  x = 0;  x < g_width;  ++x)
        {
          *p++ = src++->color.code;
/*
          auto  c = src.get_const_pixel(x,y).color;

          *p++ = c.get_r255();
          *p++ = c.get_g255();
          *p++ = c.get_b255();
*/
        }
    }
}


}




void
update_screen(const gbstd::canvas&  cv) noexcept
{
  int  pitch;

  void*  ptr;

    if(SDL_LockTexture(g_texture,nullptr,&ptr,&pitch) == 0)
    {
      transfer(cv,reinterpret_cast<uint8_t*>(ptr),pitch);

      SDL_UnlockTexture(g_texture);

      SDL_RenderClear(g_renderer);
      SDL_RenderCopy(g_renderer,g_texture,nullptr,&g_dst_rect);

      SDL_RenderPresent(g_renderer);

      gbstd::g_needed_to_redraw = false;
    }


    if(g_recording_flag)
    {
      gbpng::direct_color_image  img(g_width,g_height);

      auto  dst = img.get_row_pointer(0);
      auto  src = g_image.get_pixel_pointer(0,0);

        for(int  y = 0;  y < g_height;  ++y)
        {
            for(int  x = 0;  x < g_width;  ++x)
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


  SDL_DestroyTexture(g_texture);
  SDL_DestroyRenderer(g_renderer);
  SDL_DestroyWindow(g_window);

  SDL_Quit();

  std::_Exit(0);
}


void
init(int  w, int  h, double  scale) noexcept
{
  g_width  = w;
  g_height = h;

  g_dst_rect.x = 0;
  g_dst_rect.y = 0;
  g_dst_rect.w = w*scale;
  g_dst_rect.h = h*scale;


  SDL_Init(SDL_INIT_VIDEO);

  g_window = SDL_CreateWindow("GAME BABY - " __DATE__,SDL_WINDOWPOS_CENTERED,
                                                      SDL_WINDOWPOS_CENTERED,
                                                      g_dst_rect.w,
                                                      g_dst_rect.h,0);

    if(!g_window)
    {
      printf("cannot create window\n");
    }


  g_renderer = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
  g_texture  = SDL_CreateTexture(g_renderer,SDL_PIXELFORMAT_ARGB1555,SDL_TEXTUREACCESS_STREAMING,w,h);

  g_image.resize(w,h);
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

      SDL_DestroyTexture(g_texture);
                         g_texture  = SDL_CreateTexture(g_renderer,SDL_PIXELFORMAT_ARGB1555,SDL_TEXTUREACCESS_STREAMING,w,h);
    }


  gbstd::g_needed_to_redraw = true;
}


void
screenshot() noexcept
{
#ifndef __EMSCRIPTEN__
  auto  png = g_image.make_png_stream();

  gbstd::write_to_file(png.data(),png.size(),"__screenshot.png");
#endif
}


gbstd::canvas
make_screen_canvas() noexcept
{
  return gbstd::canvas(g_image);
}


void
start_screen_recording() noexcept
{
    if(!g_recording_flag)
    {
      gbpng::image_header  ihdr(g_width,g_height);

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




