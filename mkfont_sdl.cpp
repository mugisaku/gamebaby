#include"libmkfont/mkfont.hpp"
#include"libsdlglu/sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{



canvas
g_screen_canvas;


widgets::operating_node
g_root;


editor
g_editor(g_root);


void
main_loop() noexcept
{
  sdl::update_control();

    for(auto  pt: g_point_buffer)
    {
      g_root.process_user_input(pt);
    }


    if(g_root.redraw_if_necessary() || g_needed_to_redraw)
    {
      sdl::update_screen(g_screen_canvas);

      gbstd::g_needed_to_redraw = false;
    }
}


}


int
main(int  argc, char**  argv)
{
    if(argc != 2)
    {
      return 0;
    }


  initialize(argv[1]);

  auto&  shift_buttons = g_root.create_table_column({
    g_editor.m_shift_up_button,
    g_editor.m_shift_left_button,
    g_editor.m_shift_right_button,
    g_editor.m_shift_down_button
  });


  auto& save_buttons = g_root.create_table_row({g_editor.m_save_button,g_editor.m_save_as_combined_button});

  auto&  chrsel_buttons = g_root.create_table_row({g_editor.m_character_up_button,g_editor.m_character_down_button});
  auto&  cmbsel_buttons = g_root.create_table_row({g_editor.m_combined_up_button,g_editor.m_combined_down_button});

  auto&  bitmap_table = g_root.create_table_column({g_editor.m_bitmap,g_editor.m_selected_label});
  auto&  chrsel_table = g_root.create_table_column({g_editor.m_chrsel,g_editor.m_character_label});
  auto&  cmbsel_table = g_root.create_table_column({g_editor.m_cmbsel,cmbsel_buttons});

  auto&  chrsel_table2 = g_root.create_table_row({chrsel_table,g_editor.m_character_table_slider});

  auto&  bitmap_frame = g_root.create_frame().set_content(bitmap_table).set_string(u"canvas");
  auto&  chrsel_frame = g_root.create_frame().set_content(chrsel_table2).set_string(u"character table");
  auto&  cmbsel_frame = g_root.create_frame().set_content(cmbsel_table).set_string(u"combined table");

  auto&  upper_row = g_root.create_table_row({bitmap_frame,shift_buttons,save_buttons});
  auto&  lower_row = g_root.create_table_row({chrsel_frame,cmbsel_frame});

  g_root.add_child(g_root.create_table_column({upper_row,lower_row}),{});

  g_root.reform_if_necessary();

  sdl::init(g_root.get_width(),g_root.get_height());

  g_screen_canvas = sdl::make_screen_canvas();

  g_root.set_canvas(g_screen_canvas);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




