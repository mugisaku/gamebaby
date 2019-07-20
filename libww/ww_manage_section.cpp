#include"libww/ww_manage_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




namespace{


struct
pair
{
  std::u16string_view  name;
  std::u16string_view  description;

};


pair
g_table[] =
{
  {u"ぶきを　えらぶ",u"たたかいに　もっていく　ぶきを　えらびます\nぶきは　とちゅうで　かえることは　できません\nしんちょうに　えらびましょう"},
  {u"まほうを　えらぶ",u"たたかいに　もっていく　まほうを　えらびます\nきょうりょくな　まほうほど\nMPを　おおく　つかいます"},
  {u"したぎを　えらぶ",u"たたかいに　はいていく　したぎを　えらびます\nいつ　よろいが　ぬげても　いいように\nはずかしくないものを　えらびましょう"},
  {u"ごはんを　たべる",u"たたかいで　ちからを　はっきするためにも\nごはんを　もりもり　たべます\nただし　たべすぎると　うごきが\nにぶくなってしまうので　きをつけましょう"},
  {u"たたかいに　おもむく",u"このちに　へいおんを　とりもどすため\nモヒマンドリャどもを　おいはらいます"},

};


void
draw_sv_entry(const gbstd::menus::cell&  cell, const gbstd::canvas&  cv) noexcept
{
  auto&  p = g_table[cell.get_index()];

  cv.draw_string({gbstd::colors::white},p.name,0,0);
}


const uint8_t
g_face_png[] =
{
#include"girl.txt"
};


gbstd::image
g_face_img(g_face_png);


constexpr const char*
g_section_name = "manage";

}


manage_section::
manage_section(gbstd::execution&  exec) noexcept:
m_task(this)
{
  initialize_menu();

  startup(exec);
}




/*
void
manage_section::
draw_face(const gbstd::canvas&  cv, const gbstd::windows::style&  style, manage_section&  man) noexcept
{
  cv.draw_canvas({g_face_png,48+(48*man.m_face_animation_index),0,48,48},0,0);
}


void
manage_section::
draw_text(const gbstd::canvas&  cv, const gbstd::windows::style&  style, manage_section&  man) noexcept
{
  auto  it = man.m_text.begin();

    if(it)
    {
      auto  btm = man.m_text.bottom();
        
      int  y = 0;

        while(it != btm)
        {
//          cv.draw_string(man.m_text_color,it->get_view(),0,y);

          y += gbstd::g_font_height;

          ++it;
        }


//      cv.draw_string(man.m_text_color,it->get_view(man.m_text_cursor_position),0,y);
    }
}


void
manage_section::
manage_first_menu(menu_stack&  stk, const menu_result*  res, menu_view&  view, manage_section&  man) noexcept
{
    if(res)
    {
      auto  opv = res->get_opening_value();

        if(opv > 0)
        {
        }


      return;
    }


  auto&  cur = view.get_first_cursor();

    if(!gbstd::test_input_barrier())
    {
        if(gbstd::g_input)
        {
          gbstd::barrier_input();
        }


      auto  sv = g_table[cur.get_cell().get_index()].description;

        if(gbstd::g_input.test_p())
        {
        }

      else
        if(gbstd::g_input.test_n())
        {
          stk.close_top(0);
        }

      else
        if(gbstd::g_input.test_up())
        {
          cur.add_y(-1);

          man.clear_and_push_text(sv);
        }

      else
        if(gbstd::g_input.test_down())
        {
          cur.add_y(1);

          man.clear_and_push_text(sv);
        }
    }
}
*/


void
manage_section::
initialize_menu() noexcept
{
  m_first_menu_table.resize(1,(sizeof(g_table)/sizeof(*g_table)))
    .set_cell_width(gbstd::g_font_width*10)
    .set_cell_height(gbstd::g_font_height)
    .set_callback(draw_sv_entry)
  ;


  m_first_menu_view.set_table(m_first_menu_table);
/*
             .set_window_color(gbstd::colors::blue)
             .set_x(180)
             .set_y(20)
             .get_first_cursor().show()
*/
  ;
}


void
manage_section::
startup(gbstd::execution&  exec) noexcept
{
/*
  exec.add_clock(m_clock)
    .add_task(m_task);

  m_task.set_data(this)
    .set_clock_watch(m_clock)
    .set_interval(100)
    .set_tick(charactick);

  m_face_frame.set_content(&m_face_content)
              .set_x(0)
              .set_y(224);

  m_face_content.set_callback(draw_face,*this)
                .set_width( 48)
                .set_height(48);

  m_text_frame.set_content(&m_text_content)
              .set_x( 64)
              .set_y(208);

  m_text_content.set_callback(draw_text,*this)
                .set_width( gbstd::g_font_width *m_text_table_width )
                .set_height(gbstd::g_font_height*m_text_table_height);

  m_menu_stack.clear()
              .ready(m_clock,20)
              .open(0,m_first_menu_view,*this,manage_first_menu)
              .add_subwindow(m_face_frame)
              .add_subwindow(m_text_frame);

  m_text_color = gbstd::colors::white;

  m_text_cursor_position = 0;

  clear_text_queue();

  clear_and_push_text(g_table[0].description);
*/
}


void
manage_section::
pump_text() noexcept
{
  auto&  ptr = m_text_read_pointer;

  auto  start = ptr;

  int  length = 0;

    if(has_remain_text())
    {
        for(;;)
        {
          auto  c = *ptr;

            if(!c)
            {
              break;
            }

          else
            if(c == '\n')
            {
              ++ptr;

                if(length)
                {
                  m_text.push({start,static_cast<size_t>(length)});

                  m_text_cursor_position = 0;
                }


              break;
            }


          ++ptr;

            if(++length >= m_text_table_width)
            {
              m_text.push({start,static_cast<size_t>(length)});

              m_text_cursor_position = 0;

              break;
            }
        }
    }
}


void
manage_section::
push_text(std::u16string_view  sv) noexcept
{
    if(sv.size())
    {
      auto&  dst = m_text_write_pointer;

      auto  tail = std::end(m_text_buffer)-2;

      auto  src = sv.data();
      auto    n = sv.size();

        while((dst != tail) && n--)
        {
          *dst++ = *src++;
        }


      *dst++ = '\n';
      *dst   =    0;
    }
}


void
manage_section::
clear_and_push_text(std::u16string_view  sv) noexcept
{
  m_text.clear();

  clear_text_queue();

  push_text(sv);
}


void
manage_section::
clear_text_queue() noexcept
{
  m_text_read_pointer  = std::begin(m_text_buffer);
  m_text_write_pointer = std::begin(m_text_buffer);

  m_text_buffer[0] = 0;
}


void
manage_section::
charactick(manage_section&  man) noexcept
{
  auto  it = man.m_text.bottom();

    if(it)
    {
      auto  sv = it->get_view();

        if(man.m_text_cursor_position < sv.size())
        {
          ++man.m_text_cursor_position;

            if(++man.m_face_animation_index > 3)
            {
              man.m_face_animation_index = 0;
            }
        }

      else
        {
          man.pump_text();
        }
    }

  else
    {
      man.pump_text();
    }
}




}




