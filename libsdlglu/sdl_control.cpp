#include"sdl.hpp"
#include<SDL.h>
#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/ilc.hpp"




#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif




namespace sdl{


void  update_screen() noexcept;
void  quit() noexcept;


namespace{


void
process_key_down(const SDL_KeyboardEvent&  evt, gbstd::key_state&  keys) noexcept
{
  if(!evt.repeat)
  {
      switch(evt.keysym.sym)
      {
    case(SDLK_UP   ): keys.set_diu();break;
    case(SDLK_LEFT ): keys.set_dil();break;
    case(SDLK_RIGHT): keys.set_dir();break;
    case(SDLK_DOWN ): keys.set_did();break;

    case(SDLK_SPACE ): keys.set_opr();break;
    case(SDLK_LSHIFT):
    case(SDLK_RSHIFT):
      keys.set_shl();
      keys.set_shr();
      break;
    case(SDLK_RETURN):
    case(SDLK_z):
        keys.set_acl();
        break;
    case(SDLK_RCTRL):
    case(SDLK_LCTRL):
    case(SDLK_x    ):
        keys.set_acr();
        break;
/*
    case(SDLK_a    ):
    case(SDLK_LEFTBRACKET):
        keys.set_l();
        break;
    case(SDLK_s    ):
    case(SDLK_RIGHTBRACKET):
        keys.set_r();
        break;
*/
      }
  }
}


void
process_key_up(const SDL_KeyboardEvent&  evt, gbstd::key_state&  keys) noexcept
{
    switch(evt.keysym.sym)
    {
  case(SDLK_UP   ): keys.unset_diu();break;
  case(SDLK_LEFT ): keys.unset_dil();break;
  case(SDLK_RIGHT): keys.unset_dir();break;
  case(SDLK_DOWN ): keys.unset_did();break;

  case(SDLK_SPACE ): keys.unset_opr();break;
  case(SDLK_LSHIFT):
  case(SDLK_RSHIFT):
   keys.unset_shl();
   keys.unset_shr();
   break;
  case(SDLK_RETURN):
  case(SDLK_z     ):
      keys.unset_acl();
      break;
  case(SDLK_RCTRL):
  case(SDLK_LCTRL):
  case(SDLK_x    ):
      keys.unset_acr();
      break;
/*
    case(SDLK_a    ):
    case(SDLK_LEFTBRACKET):
        keys.unset_acl();
        break;
    case(SDLK_s    ):
    case(SDLK_RIGHTBRACKET):
        keys.unset_acr();
        break;
*/
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
process_mouse(const SDL_MouseButtonEvent&  evt, gbstd::key_state&  keys) noexcept
{
  gbstd::update_point({evt.x,evt.y});

    if(evt.state == SDL_PRESSED)
    {
           if(evt.button == SDL_BUTTON_LEFT ){keys.set_mol();}
      else if(evt.button == SDL_BUTTON_RIGHT){keys.set_mor();}
    }

  else
    {
           if(evt.button == SDL_BUTTON_LEFT ){keys.unset_mol();}
      else if(evt.button == SDL_BUTTON_RIGHT){keys.unset_mor();}
    }
}


void
process_mouse_motion(const SDL_MouseMotionEvent&  evt, gbstd::key_state&  keys) noexcept
{
  gbstd::update_point({evt.x,evt.y});

    if(evt.state&SDL_BUTTON_LMASK){keys.set_mol();}
  else                            {keys.unset_mol();}

    if(evt.state&SDL_BUTTON_RMASK){keys.set_mor();}
  else                            {keys.unset_mor();}
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
process_event(const SDL_Event&  evt, gbstd::key_state&  keys) noexcept
{
    switch(evt.type)
    {
  case(SDL_KEYDOWN): process_key_down(evt.key,keys);break;
  case(SDL_KEYUP  ): process_key_up(  evt.key,keys);break;
  case(SDL_MOUSEBUTTONUP  ): process_mouse(evt.button,keys);break;
  case(SDL_MOUSEBUTTONDOWN): process_mouse(evt.button,keys);break;
  case(SDL_MOUSEMOTION): process_mouse_motion(evt.motion,keys);break;
  case(SDL_WINDOWEVENT):
         switch(evt.window.event)
         {
       case(SDL_WINDOWEVENT_EXPOSED):
           gbstd::set_redraw_flag();
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




void
update_control() noexcept
{
  static SDL_Event  evt;

  auto  keys = gbstd::get_pressed_keys();

#ifdef __EMSCRIPTEN__
  try_read_dropped_file();
#endif

    while(SDL_PollEvent(&evt))
    {
      process_event(evt,keys);
    }


  gbstd::update_keys(keys);

  gbstd::update_time(SDL_GetTicks());

  update_screen();
}


}


void
start_loop() noexcept
{
  static bool  started;

    if(!started)
    {
      started = true;

#ifdef __EMSCRIPTEN__
      emscripten_set_main_loop(update_control,0,false);
#else
        for(;;)
        {
          update_control();

          SDL_Delay(20);
        }
#endif
    }
}




}




