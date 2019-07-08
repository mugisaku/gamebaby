#include"libge/color_handler.hpp"
#include"libge/core.hpp"
#include"libge/ge.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


ge::core_display*
g_display;


class
cell_menu: public widgets::menu_handler
{
public:
  cell_menu(widgets::operating_node&  root) noexcept: menu_handler(root){}

  void  render_background(const gbstd::canvas&  cv) noexcept override
  {
    g_display->render_background(cv,2);
  }
};


canvas
g_canvas;


widgets::operating_node
g_root;


widgets::menu_handler*
g_menu;


menus::table
g_table;


menus::view
g_view;


ge::core*
g_core;


ge::aniview*
g_aniv;


constexpr int  g_cell_size = 48;
constexpr int g_pixel_size =  6;
constexpr int  g_initial_w = 6;
constexpr int  g_initial_h = 2;


void
main_loop() noexcept
{
  sdl::update_control();

  auto&  f = g_dropped_file;

    if(f.size())
    {
      g_view.set_x_offset(0);
      g_view.set_y_offset(0);

      g_core->get_source().load(f);

      f.clear();
    }


    for(auto&  pt: g_point_buffer)
    {
      g_root.process_user_input(pt);
    }


  g_aniv->check();

    if(g_root.redraw_if_necessary() || g_needed_to_redraw)
    {
      sdl::update_screen(g_canvas);
    }
}


gbstd::widgets::node&
create_cell_menu_widget() noexcept
{
  g_menu = new cell_menu(g_root);

  auto&  menu = *g_menu;

  menu
    .set_view(g_view)
    .set_callback([](widgets::menu_event  evt){
        if(evt.is_press())
        {
          auto  pos = evt->get_current_cell().get_position();

          g_core->set_pixel_base_point(pos);
        }
    })
  ;


  auto&  upbtn = menu.create_up_button(g_root);
  auto&  dwbtn = menu.create_down_button(g_root);

  auto&  btns = g_root.create_table_column({upbtn,dwbtn});

  auto&  szlbl = g_core->get_source().get_size_label();
  auto&  yolbl = menu.get_y_offset_label();
  auto&  exbtn = g_core->get_source().create_extend_widget(g_root);

  auto&  mnu = g_root.create_table_row({menu,btns});

  return g_root.create_frame(u"cell table").set_content(g_root.create_table_column({
    szlbl,
    mnu,
    g_root.create_table_row({exbtn,yolbl})
  }));
}


}


int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  set_caption("mkanigra - " __DATE__);
  set_description("<pre>"
                  "*マウスの左ボタンで、任意色を置き、\n"
                  " 右ボタンで透明色を置く\n"
                  "*画像の編集単位をセルと呼ぶ\n"
                  "*PNGファイルをドラッグ・アンド・ドロップで読み込む\n"
                  "◇tool\n"
                  "  *fill areaは指定地点と同色の領域を新たな色で塗りつぶす\n"
                  "  *selectはundoを除くoperationの有効範囲を変更する\n"
                  "   開始時は全体が範囲\n"
                  "  *select時、右クリックで範囲を全体に戻す\n"
                  "  *pasteは保持されたセルを透明色も含めて貼り付ける\n"
                  "  *layerは保持されたセルを透明色を除いて貼り付ける\n"
                  "◇operation\n"
                  "  *undoは最後の編集操作を取り消す。セルを移ると編集操作記録はクリアされる\n"
                  "  *copyは現在のセルから指定範囲を複製して保持する\n"
                  "◇animation\n"
                  "  *pushはアニメーションの最後尾に現在のセルを付け加える\n"
                  "  *popはアニメーションの最後尾から、ひとつ取り除く\n"
                  "◇underlay\n"
                  "  *pushは下敷きの最上段に現在のセルを乗せる\n"
                  "  *popは下敷きの最上段から、ひとつ取り除く\n"
                  "  *show/hideは下敷きの有効/無効を切り替える\n"
                  "◇save as png\n"
                  "  *セルテーブルを一枚のPNG形式で出力する\n"
                  "  *背景色と下敷きは反映されない\n"
                  "◇save as apng\n"
                  "  *アニメーションの内容をAPNG形式で出力する\n"
                  "  *背景色と下敷き（有効なら）が反映される\n"
                  "◇save as C code\n"
                  "  *セルテーブルを一枚のPNG形式にし、C言語の配列リテラルとして出力する\n"
                  "  *詳細はPNGで出力するのと同じ\n"
                  "</pre>"
  );


  show_github_link();
  show_twitter_link();

#endif
  g_table
    .set_cell_width(g_cell_size)
    .set_cell_height(g_cell_size)
    .resize(g_initial_w,g_initial_h)
    .set_callback([](const menus::cell&  cell, const canvas&  cv){
      auto  pos = cell.get_position();

      cv.draw_canvas(g_core->get_source().get_canvas(pos),0,0);
    })
  ;


  g_view
    .set_table(g_table)
    .set_width(g_initial_w)
    .set_height(g_initial_h)
  ;


  g_core = new ge::core(g_root);

  auto&  src = g_core->get_source();


  g_aniv = new ge::aniview(g_root,*g_core);


  auto&  stk = g_core->create_underlay_widget(g_root);

  auto&  tools = g_core->get_paint().create_tool_widget(g_root);
  auto&    ops = g_core->get_paint().create_operation_widget(g_root);

  g_display = &g_core->get_display().set_pixel_size(g_pixel_size);

  auto&  bgop = g_display->create_background_widget(g_root);

  auto&  rightest_col = g_root.create_table_column({ops,bgop});

  auto  colhndl = ge::make_color_handler(g_root,g_core->get_paint().get_drawing_color());

  auto&  celmnu = create_cell_menu_widget();

  src
    .set_menu(*g_menu)
    .set_cell_size(g_cell_size,g_cell_size)
    .inflate(g_initial_w,g_initial_h)
  ;

  g_core->get_paint().reset();

  auto&  savbtns = g_root.create_table_column({
    src.create_save_png_widget(g_root),
    src.create_save_txt_widget(g_root),
  });


  g_core->add_coworker({celmnu});


  auto&  upper_row = g_root.create_table_row({g_core->create_frame(g_root),colhndl.maker,colhndl.holder,tools,rightest_col});
  auto&  lower_row = g_root.create_table_row({celmnu,g_aniv->create_frame(g_root),stk,savbtns});


  g_root.add_child_as_column({upper_row,lower_row});

  g_root.reform_if_necessary();

  sdl::init(g_root.get_width(),g_root.get_height());

  g_canvas = sdl::make_screen_canvas();

  g_root.set_canvas(g_canvas);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }
#endif


  return 0;
}




