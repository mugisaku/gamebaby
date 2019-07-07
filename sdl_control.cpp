#include"sdl.hpp"
#include<SDL.h>
#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"


namespace sdl{


namespace{


int
g_mouse_counter;


gbstd::point
g_mouse_pos;


void
process_key_down(const SDL_KeyboardEvent&  evt) noexcept
{
  if(!evt.repeat)
  {
      switch(evt.keysym.sym)
      {
    case(SDLK_UP   ): gbstd::g_input.set_up();break;
    case(SDLK_LEFT ): gbstd::g_input.set_left();break;
    case(SDLK_RIGHT): gbstd::g_input.set_right();break;
    case(SDLK_DOWN ): gbstd::g_input.set_down();break;

    case(SDLK_SPACE ): gbstd::g_input.set_start();break;
    case(SDLK_LSHIFT):
    case(SDLK_RSHIFT): gbstd::g_input.set_shift();break;

    case(SDLK_RETURN):
    case(SDLK_z):
        gbstd::g_input.set_p();
        break;
    case(SDLK_RCTRL):
    case(SDLK_LCTRL):
    case(SDLK_x    ):
        gbstd::g_input.set_n();
        break;
    case(SDLK_a    ):
    case(SDLK_LEFTBRACKET):
        gbstd::g_input.set_l();
        break;
    case(SDLK_s    ):
    case(SDLK_RIGHTBRACKET):
        gbstd::g_input.set_r();
        break;
      }
  }
}


void
process_key_up(const SDL_KeyboardEvent&  evt) noexcept
{
    switch(evt.keysym.sym)
    {
  case(SDLK_UP   ): gbstd::g_input.unset_up();break;
  case(SDLK_LEFT ): gbstd::g_input.unset_left();break;
  case(SDLK_RIGHT): gbstd::g_input.unset_right();break;
  case(SDLK_DOWN ): gbstd::g_input.unset_down();break;

  case(SDLK_SPACE ): gbstd::g_input.unset_start();break;
  case(SDLK_LSHIFT):
  case(SDLK_RSHIFT): gbstd::g_input.unset_shift();break;

  case(SDLK_RETURN):
  case(SDLK_z     ):
      gbstd::g_input.unset_p();
      break;
  case(SDLK_RCTRL):
  case(SDLK_LCTRL):
  case(SDLK_x    ):
      gbstd::g_input.unset_n();
      break;
    case(SDLK_a    ):
    case(SDLK_LEFTBRACKET):
        gbstd::g_input.unset_l();
        break;
    case(SDLK_s    ):
    case(SDLK_RIGHTBRACKET):
        gbstd::g_input.unset_r();
        break;
  case(SDLK_t):
      screenshot();
      break;
  case(SDLK_r):
        if(test_screen_recording()){  end_screen_recording();}
      else                         {start_screen_recording();}
      break;
    }
}


void
process_mouse(const SDL_MouseButtonEvent&  evt) noexcept
{
  g_mouse_pos = gbstd::point(evt.x,evt.y);

  ++g_mouse_counter;

    if(evt.state == SDL_PRESSED)
    {
           if(evt.button == SDL_BUTTON_LEFT ){gbstd::g_input.set_mouse_left();}
      else if(evt.button == SDL_BUTTON_RIGHT){gbstd::g_input.set_mouse_right();}
    }

  else
    {
           if(evt.button == SDL_BUTTON_LEFT ){gbstd::g_input.unset_mouse_left();}
      else if(evt.button == SDL_BUTTON_RIGHT){gbstd::g_input.unset_mouse_right();}
    }
}


void
process_mouse_motion(const SDL_MouseMotionEvent&  evt) noexcept
{
  g_mouse_pos = gbstd::point(evt.x,evt.y);

  ++g_mouse_counter;

    if(evt.state&SDL_BUTTON_LMASK){gbstd::g_input.set_mouse_left();}
  else                            {gbstd::g_input.unset_mouse_left();}

    if(evt.state&SDL_BUTTON_RMASK){gbstd::g_input.set_mouse_right();}
  else                            {gbstd::g_input.unset_mouse_right();}
}


#ifdef __EMSCRIPTEN__
void
try_read_dropped_file() noexcept
{
    if(gbstd::g_dropped_file.empty() && gbstd::get_number_of_dropped_files())
    {
      gbstd::g_dropped_file = gbstd::pop_front_dropped_file();
    }
}
#else
void
read_dropped_file(const char*  filepath) noexcept
{
  auto  f = fopen(filepath,"rb");

    if(f)
    {
      gbstd::g_dropped_file.clear();

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          gbstd::g_dropped_file.emplace_back(c);
        }


      fclose(f);
    }
}
#endif


void
process_event(const SDL_Event&  evt) noexcept
{
    switch(evt.type)
    {
  case(SDL_KEYDOWN): process_key_down(evt.key);break;
  case(SDL_KEYUP  ): process_key_up(  evt.key);break;
  case(SDL_MOUSEBUTTONUP  ): process_mouse(evt.button);break;
  case(SDL_MOUSEBUTTONDOWN): process_mouse(evt.button);break;
  case(SDL_MOUSEMOTION): process_mouse_motion(evt.motion);break;
  case(SDL_WINDOWEVENT):
         switch(evt.window.event)
         {
       case(SDL_WINDOWEVENT_EXPOSED):
           gbstd::g_needed_to_redraw = true;
           break;
         }
       break;
#ifndef __EMSCRIPTEN__
  case(SDL_DROPFILE):
        read_dropped_file(evt.drop.file);

        SDL_free(evt.drop.file);
        break;
#endif
  case(SDL_QUIT):
       quit();
       break;
    }
}
}


void
delay(uint32_t  ms) noexcept
{
  SDL_Delay(ms);
}


void
update_control() noexcept
{
  static SDL_Event  evt;

  static gbstd::user_input  previous_input                 ;
                            previous_input = gbstd::g_input;

  gbstd::g_time = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
  try_read_dropped_file();
#endif

  gbstd::g_point_buffer.clear();

  gbstd::point  tmp_mouse_pos = g_mouse_pos;

  g_mouse_counter = 0;

    while(SDL_PollEvent(&evt))
    {
      process_event(evt);
    }


    if(g_mouse_counter)
    {
      gbstd::liner  liner(tmp_mouse_pos,g_mouse_pos);

        for(;;)
        {
          gbstd::g_point_buffer.emplace_back(liner.get_x(),liner.get_y());

            if(!liner.get_distance())
            {
              break;
            }


          liner.step();
        }
    }


  gbstd::g_modified_input = previous_input^gbstd::g_input;

    if(gbstd::g_modified_input)
    {
      gbstd::barrier_input(0);
    }
}


}




