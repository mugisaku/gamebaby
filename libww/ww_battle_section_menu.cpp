#include"libww/ww_battle_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




namespace{


std::u16string_view  attack_sv(u"こうげき");
std::u16string_view   guard_sv(u"ぼうぎょ");
std::u16string_view    move_sv(u"いどう");

std::u16string_view  advance_sv(u"ぜんしん");
std::u16string_view     back_sv(u"こうたい");
std::u16string_view     stop_sv(u"ていし");

std::u16string_view    fastly_sv(u"はやく");
std::u16string_view  normally_sv(u"ふつう");
std::u16string_view    slowly_sv(u"おそく");

std::u16string_view  strongly_sv(u"つよく");
std::u16string_view    weakly_sv(u"よわく");



void
draw_sv_entry(const gbstd::menus::cell&  cell, const gbstd::canvas&  cv) noexcept
{
  cv.draw_string(gbstd::colors::white,cell.get_string(),0,0);
}


}




/*
void
battle_section::
manage_first_menu(gbstd::menus::stack&  stk, const gbstd::menus::result*  res, gbstd::menus::view&  view, battle_section&  b) noexcept
{
    if(res)
    {
      auto  opv = res->get_opening_value();

        if(opv > 0)
        {
            for(auto&  ln: b.m_line_table)
            {
              auto&  p = ln.m_piece;

              auto&  i = (opv == 1)? p.m_offense_intensity
                        :            p.m_defense_intensity;

                switch(res->get_closing_value())
                {
              case(0): i = intensity::do_strongly;break;
              case(1): i = intensity::do_normally;break;
              case(2): i = intensity::do_weakly  ;break;
                }
            }
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


        if(gbstd::g_input.test_p())
        {
          b.m_second_menu_view.get_first_cursor().set_y(0);

          auto  p = b.m_second_menu_table.get_cell_pointer(0,0);

            if(cur.get_y() == 0)
            {
              p++->set_string(advance_sv);
              p++->set_string(   back_sv);
              p++->set_string(   stop_sv);

              stk.open(cur.get_y(),b.m_second_menu_view,b,battle_section::manage_footwork_menu);
            }

          else
            {
              p++->set_string(strongly_sv);
              p++->set_string(normally_sv);
              p++->set_string(  weakly_sv);

              stk.open(cur.get_y(),b.m_second_menu_view,b,battle_section::manage_handwork_menu);
            }
        }

      else
        if(gbstd::g_input.test_n()    ){stk.close_top(0);}
        if(gbstd::g_input.test_up()   ){cur.add_y(-1);}
        if(gbstd::g_input.test_down() ){cur.add_y( 1);}
    }
}


void
battle_section::
manage_handwork_menu(gbstd::menus::stack&  stk, const gbstd::menus::result*  res, gbstd::menus::view&  view, battle_section&  b) noexcept
{
    if(res)
    {
      return;
    }


  auto&  cur = view.get_first_cursor();

    if(!gbstd::test_input_barrier())
    {
        if(gbstd::g_input)
        {
          gbstd::barrier_input();
        }


        if(gbstd::g_input.test_p())
        {
          stk.close_top(cur.get_y());
        }

      else
        if(gbstd::g_input.test_n()    ){stk.close_top(-1);}
        if(gbstd::g_input.test_up()   ){cur.add_y(-1);}
        if(gbstd::g_input.test_down() ){cur.add_y( 1);}
    }
}


void
battle_section::
manage_footwork_menu(gbstd::menus::stack&  stk, const gbstd::menus::result*  res, gbstd::menus::view&  view, battle_section&  b) noexcept
{
    if(res)
    {
        for(auto&  ln: b.m_line_table)
        {
          auto&  p = ln.m_piece;

          auto&  movi = p.m_movement_intensity;
          auto&  movd = p.m_move_direction;

            switch(res->get_closing_value())
            {
          case(0): movi = intensity::do_strongly;break;
          case(1): movi = intensity::do_normally;break;
          case(2): movi = intensity::do_weakly  ;break;
            }


            switch(res->get_opening_value())
            {
          case(0): movd = p.m_side               ;break;
          case(1): movd = p.m_side.get_opposite();break;
            }
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


        if(gbstd::g_input.test_p())
        {
            if(cur.get_y() <= 1)
            {
              b.m_third_menu_view.get_first_cursor().set_y(0);

              auto  p = b.m_third_menu_table.get_cell_pointer(0,0);

              p++->set_string(  fastly_sv);
              p++->set_string(normally_sv);
              p++->set_string(  slowly_sv);

              stk.open(cur.get_y(),b.m_third_menu_view,b,battle_section::manage_third_menu);
            }

          else
            {
                for(auto&  ln: b.m_line_table)
                {
                  ln.m_piece.m_movement_intensity = intensity::do_not;
                }


              stk.close_top(-1);
            }
        }

      else
        if(gbstd::g_input.test_n()    ){stk.close_top(-1);}
        if(gbstd::g_input.test_up()   ){cur.add_y(-1);}
        if(gbstd::g_input.test_down() ){cur.add_y( 1);}
    }
}


void
battle_section::
manage_third_menu(gbstd::menus::stack&  stk, const gbstd::menus::result*  res, gbstd::menus::view&  view, battle_section&  b) noexcept
{
    if(res)
    {
      return;
    }


  auto&  cur = view.get_first_cursor();

    if(!gbstd::test_input_barrier())
    {
        if(gbstd::g_input)
        {
          gbstd::barrier_input();
        }


        if(gbstd::g_input.test_p())
        {
          stk.close_top(cur.get_y());
        }

      else
        if(gbstd::g_input.test_n()    ){stk.close_top(-1);}
        if(gbstd::g_input.test_up()   ){cur.add_y(-1);}
        if(gbstd::g_input.test_down() ){cur.add_y( 1);}
    }
}
*/


void
battle_section::
initialize_menu() noexcept
{
/*
  m_first_menu_table
    .resize(1,3)
    .set_cell_width(gbstd::g_font_width*5)
    .set_cell_height(gbstd::g_font_height)
    .set_callback(draw_sv_entry)
  ;

  m_second_menu_table
    .resize(1,3)
    .set_cell_width(gbstd::g_font_width*5)
    .set_cell_height(gbstd::g_font_height)
    .set_callback(draw_sv_entry)
  ;

  m_third_menu_table
    .resize(1,3)
    .set_cell_width(gbstd::g_font_width*5)
    .set_cell_height(gbstd::g_font_height)
    .set_callback(draw_sv_entry)
  ;

  {
    auto  p = m_first_menu_table.get_cell_pointer(0,0);

    p++->set_string(  move_sv);
    p++->set_string(attack_sv);
    p++->set_string( guard_sv);
  }
*/

  m_first_menu_view
/*
    .assign(m_first_menu_table)
    .set_window_color(gbstd::colors::blue)
    .set_x(32)
    .set_y(180)
    .get_first_cursor()
    .show()
*/
  ;


  m_second_menu_view
/*
    .assign(m_second_menu_table)
    .set_window_color(gbstd::colors::blue)
    .set_x(32+80)
    .set_y(180)
    .get_first_cursor()
    .show()
*/
  ;


/*
  m_third_menu_view
    .assign(m_third_menu_table)
    .set_window_color(gbstd::colors::blue)
    .set_x(32+140)
    .set_y(180)
    .get_first_cursor()
    .show()
*/
  ;
}



}




