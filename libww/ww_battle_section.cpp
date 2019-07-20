#include"libww/ww_battle_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




namespace{


void
initialize(piece&  p, int  side_counter) noexcept
{
  bool  flag = (side_counter&1);

  p.m_status.clear();
  p.m_side  = flag? battles::sides::left:battles::sides::right;
  p.m_position  = flag? 24.5:(context::get_screen_width()-1-24.5);
  p.m_previous_position = p.m_position;

  p.m_offense_intensity  = intensity::do_normally;
  p.m_defense_intensity  = intensity::do_normally;
  p.m_movement_intensity = intensity::do_not;

  p.m_offense_power_base  = 20.0;
  p.m_defense_power_base  = 10.0;
  p.m_movement_power_base =  1.0;
  p.m_weight              =  1.0;

  p.m_hp  = 800;

  p.m_action_counter          = 0;
  p.m_animation_index         = 0;
  p.m_animation_step_counter  = 0;
  p.m_animation_frame_counter = 0;

  p.m_body_direction = p.m_side;
  p.m_move_direction = p.m_side;
}


constexpr const char*
g_section_name = "battle";


}




battle_section::
battle_section(gbstd::execution&  exec) noexcept:
m_task(this)
{
  initialize_menu();

  reset();
}


battle_section::
~battle_section()
{
  clear();
}




namespace{
const uint8_t  g_human_png[] =
{
#include"gra.png.txt"
};


gbstd::image
gra_img(g_human_png);


void
draw(const gbstd::canvas&  cv, piece&  p) noexcept
{
  constexpr int  size = 16;

  int  x = static_cast<int>(p.m_position);

  gbstd::canvas  src(gra_img,p.m_image_rect);

    if(p.m_body_direction.is_left()){cv.draw_canvas(          src,x-24,p.m_line->m_y_position-48);}
  else                              {cv.draw_canvas_reversely(src,x-24,p.m_line->m_y_position-48);}
}


void
bdraw(const gbstd::canvas&  cv, battle_section&  b) noexcept
{
    for(int  i = 0;  i < b.m_number_of_lines;  ++i)
    {
      auto&  ln = b.m_line_table[i];

      draw(cv,ln.m_piece);
    }
}


void
tick(piece&  p) noexcept
{
++p.m_action_counter;
++p.m_animation_step_counter;

  constexpr int   move = 0;
  constexpr int   down = 1;
  constexpr int  dance = 2;

  constexpr int  table[] = {0,48,0,96};

  p.m_image_rect.y = 48*p.m_animation_index;
  p.m_image_rect.w = 48;
  p.m_image_rect.h = 48;

    switch(p.m_animation_index)
    {
  case(move ):
      p.m_animation_frame_counter = (p.m_animation_step_counter>>1)&3;
      p.m_image_rect.x = table[p.m_animation_frame_counter];
      break;
  case(dance):
      p.m_animation_frame_counter = (p.m_animation_step_counter>>1)&3;
      p.m_image_rect.x = table[p.m_animation_frame_counter];
      break;
  case(down):
        if(p.m_animation_frame_counter < 2)
        {
            if(p.m_animation_step_counter > 8)
            {
              p.m_animation_step_counter   = 0;
              p.m_animation_frame_counter += 1;
            }


          p.m_image_rect.x = 48*p.m_animation_frame_counter;
        }
      break;
    }


if(p.m_hp <= 0){return;}
  p.m_previous_position = p.m_position;

  static gbstd::normal_rand    weak_r(0.4,0.3);
  static gbstd::normal_rand  normal_r(0.9,0.3);
  static gbstd::normal_rand  strong_r(1.4,0.3);

  double  off_rate = (p.m_offense_intensity == intensity::do_weakly  )? weak_r()
                    :(p.m_offense_intensity == intensity::do_normally)? normal_r()
                    :(p.m_offense_intensity == intensity::do_strongly)? strong_r()
                    :0;

  double  def_rate = (p.m_defense_intensity == intensity::do_weakly  )? weak_r()
                    :(p.m_defense_intensity == intensity::do_normally)? normal_r()
                    :(p.m_defense_intensity == intensity::do_strongly)? strong_r()
                    :0;

  double  mov_rate = (p.m_movement_intensity == intensity::do_weakly  )? weak_r()
                    :(p.m_movement_intensity == intensity::do_normally)? normal_r()
                    :(p.m_movement_intensity == intensity::do_strongly)? strong_r()
                    :0;


  p.m_offense_power  = p.m_offense_power_base *off_rate;
  p.m_defense_power  = p.m_defense_power_base *def_rate;
  p.m_movement_power = p.m_movement_power_base*mov_rate;

  constexpr double  stroke_base = 2.0;

  double  stroke = stroke_base*p.m_movement_power;

    if(p.m_move_direction.is_left()){p.m_position += stroke;}
  else                              {p.m_position -= stroke;}


  p.m_status.clear();
}


void
fix(piece&  p) noexcept
{
    if(p.m_animation_index)
    {
      return;
    }


    if(p.m_position <= 24.0)
    {
      p.m_position = 24.0;

        if(p.m_side.is_left() && p.m_status.test(piece::flags::be_pressed_from_right)){p.m_hp = 0;}
      else
        if(p.m_side.is_right())
        {
          p.m_movement_intensity = intensity::do_not;
          p.m_animation_index    = 2;
        }
    }

  else
    if(p.m_position >= (context::get_screen_width()-24.0))
    {
      p.m_position = context::get_screen_width()-24.0;

        if(p.m_side.is_right() && p.m_status.test(piece::flags::be_pressed_from_left)){p.m_hp = 0;}
      else
        if(p.m_side.is_left())
        {
          p.m_movement_intensity = intensity::do_not;
          p.m_animation_index    = 2;
        }
    }


    if((p.m_hp <= 0) && (p.m_animation_index != 1))
    {
      p.m_movement_intensity = intensity::do_not;

      p.m_animation_index = 1;
      p.m_animation_step_counter = 0;
      p.m_animation_frame_counter = 0;
    }
}


void
process(piece&  l, piece&  r) noexcept
{
    if((l.m_hp > 0) && (r.m_hp > 0))
    {
      auto  ll = l.minpos()-8;
      auto  lr = l.maxpos()+8;
      auto  rl = r.minpos()-8;
      auto  rr = r.maxpos()+8;

        if((lr > rl) && (ll < rr))
        {
          auto  r_off = r.m_offense_power-l.m_defense_power;
          auto  l_off = l.m_offense_power-r.m_defense_power;

            if(r_off < 0){r_off = 0.1;}
            if(l_off < 0){l_off = 0.1;}

          l.m_hp -= r_off;
          r.m_hp -= l_off;

            if((l.m_hp > 0) && (r.m_hp > 0))
            {
              auto  lmax = std::max(ll,rl);
              auto  rmin = std::min(lr,rr);

              auto  lw = l.m_weight+(l.m_defense_power/2)-(r.m_offense_power/2);
              auto  rw = r.m_weight+(r.m_defense_power/2)-(l.m_offense_power/2);

              auto  l_dist = std::abs(l.m_position-l.m_previous_position);
              auto  r_dist = std::abs(r.m_position-r.m_previous_position);

              l_dist *= (lw < rw)? (lw/rw):1.0        ;
              r_dist *= (lw < rw)?         1.0:(lw/rw);

              auto  m = lmax+((rmin-lmax)/2);

    //                  auto  m = ((l.m_previous_position+l_dist)
    //                            +(r.m_previous_position-r_dist))/2;

              l.m_position = m-8;
              r.m_position = m+8;

              l.m_status.set(piece::flags::be_pressed_from_right);
              r.m_status.set(piece::flags::be_pressed_from_left );
            }
        }
    }


  fix(l);
  fix(r);
}


void
btick(battle_section&  b) noexcept
{
    for(int  i = 0;  i < b.m_number_of_lines;  ++i)
    {
      auto&  ln = b.m_line_table[i];

      tick(ln.m_piece);
    }


  field_line*  lnptr0 = b.m_line_table  ;
  field_line*  lnptr1 = b.m_line_table+1;

  process(lnptr1->m_piece,lnptr0->m_piece);

  int  n = (b.m_number_of_lines/2)-1;

    for(int  i = 0;  i < n;  ++i)
    {
      lnptr0 += 2;

      process(lnptr1->m_piece,lnptr0->m_piece);

      lnptr1 += 2;

      process(lnptr1->m_piece,lnptr0->m_piece);
    }


/*
    if(gbstd::g_input.test_p() && !b.m_first_menu_view.test_busy_flag())
    {
      b.m_menu_stack.ready(b.m_clock,20);

      gbstd::barrier_input();

      b.m_first_menu_view.get_first_cursor().set_y(0);

      b.m_menu_stack.open(0,b.m_first_menu_view,b,battle_section::manage_first_menu);
    }
*/
}
}


void
battle_section::
entry_party(const party&  p, battles::side  side) noexcept
{
  int&  n = (side.is_left())? m_number_of_left_characters
           :                  m_number_of_right_characters;

    for(int  i = 0;  i < p.m_number_of_members;  ++i)
    {
      auto&  c = m_character_table[m_number_of_total_characters++];

      auto&  pm = p.m_member_table[i];

      c.m_head = pm.m_head;
      c.m_original_body = pm.m_body;
      c.copy_body();
      c.m_status.set(battles::character::flags::active);


      constexpr int  line_height = 48;

      c.m_hp_bar.set_position(side.is_left()? 0:context::get_screen_width(),line_height*n+16)
                .set_mode(side.is_left()? bar::mode::left_to_right: bar::mode::right_to_left)
                .set_thickness(10)
                .set_length(0)
                .set_desired_length(0)
                .update_increment();

      c.m_ap_bar.set_position(side.is_left()? 0:context::get_screen_width(),line_height*n+26)
                  .set_mode(side.is_left()? bar::mode::left_to_right: bar::mode::right_to_left)
                  .set_color(gbstd::colors::blue)
                  .set_thickness(4)
                  .set_length(0)
                  .set_desired_length(0)
                  .update_increment();

      c.m_hp_display   = &c.m_hp_string;
      c.m_name_display = &c.m_name_string;

      c.m_name_string.set_string(c.m_head.m_name)
                     .set_color(gbstd::colors::white);

        if(side.is_left()){c.m_name_display.set_align_to_left();}
      else                {c.m_name_display.set_align_to_right();}


      c.m_name_display.set_position({side.is_left()? 0:context::get_screen_width(),line_height*n});

      c.m_side = side;

/*
      m_task_list.push(c).set_draw<battles::character>().set_tick<battles::character>(m_clock_watch,20);

      m_task_list.push(  c.m_hp_display).set_draw<string_display>();
      m_task_list.push(c.m_name_display).set_draw<string_display>();
*/

      ++n;
    }
}


void
battle_section::
clear() noexcept
{
  m_number_of_total_characters = 0;

  m_number_of_left_characters  = 0;
  m_number_of_right_characters = 0;

  m_number_of_playing_characters = 0;

    for(auto&  c: m_character_table)
    {
      c.m_status.clear();
    }


  m_window_stack.clear();
}


void
battle_section::
reset() noexcept
{
  int  n = 48;

  m_number_of_lines = 1;
  m_number_of_lines = 8;

  int  side_counter = 0;

    for(int  i = 0;  i < m_number_of_lines;  ++i)
    {
      auto&  ln = m_line_table[i];

      ln.m_y_position = n      ;
                        n += 10;


      ln.m_piece.m_line  = &ln;

      initialize(ln.m_piece,side_counter++);
    }


//  m_task_list.push(*this).set_draw(bdraw).set_tick(m_clock_watch,80,btick);
}




}




