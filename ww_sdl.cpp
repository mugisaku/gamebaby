#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libww/ww_system.hpp"
#include"libsdlglu/sdl.hpp"
#include<cstring>




#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas
g_screen_canvas;


ww::system  
g_system;


process
g_process;


void
main_loop() noexcept
{
  sdl::update_control();

    if(g_process())
    {
      sdl::update_screen(g_screen_canvas);
    }
}


}




int
main(int  argc, char**  argv)
{
#ifdef __EMSCRIPTEN__
  set_description("<pre>"
                  "</pre>");

  show_github_link();
#endif

  sdl::init(ww::context::get_screen_width(),ww::context::get_screen_height(),1.0);

  g_screen_canvas = sdl::make_screen_canvas();

    if(argc == 2)
    {
        if(std::strcmp(argv[1],"-rec") == 0)
        {
          sdl::start_screen_recording();
        }
    }


  auto&  ctx = g_system.create_context();

  g_process.push(ctx)
    .set_interval(20)
    .set_canvas(g_screen_canvas);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit_sound();
  sdl::quit();
#endif


  return 0;
}




