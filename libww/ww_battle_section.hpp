#ifndef ww_battle_section_hpp_is_included
#define ww_battle_section_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include"libgbstd/menu.hpp"
#include<list>
#include<vector>




namespace ww{




template<typename  T, int  N>
class
filtering_table
{
  T*  m_buffer[N];

  int  m_number=0;

public:
  filtering_table() noexcept: m_number(0){}

  template<typename  CONTAINER>
  filtering_table(CONTAINER&  c) noexcept{assign(c);}

  template<typename  IT>
  filtering_table(IT  begin_, IT  end_) noexcept{assign(begin_,end_);}

  template<typename  CONTAINER>
  filtering_table&  assign(CONTAINER&  c) noexcept{return assign(std::begin(c),std::end(c));}

  template<typename  IT>
  filtering_table&  assign(IT  begin_, IT  end_) noexcept
  {
    m_number = 0;

      while(begin_ != end_)
      {
        push(&*begin_++);
      }


    return *this;
  }

  template<typename  TESTER>
  filtering_table&  filter(TESTER  tester) noexcept
  {
    filtering_table  tmp(begin(),end());

    m_number = 0;

      for(auto  ptr: tmp)
      {
          if(tester(*ptr))
          {
            push(ptr);
          }
      }


    return *this;
  }

  template<typename  CMP>
  filtering_table&  sort(CMP  cmp) noexcept
  {
    std::stable_sort(begin(),end(),cmp);

    return *this;
  }

  T&  operator[](int  i) const noexcept{return *m_buffer[i];}

  T**  begin() noexcept{return &m_buffer[       0];}
  T**    end() noexcept{return &m_buffer[m_number];}

  void  push(T*   t) noexcept{m_buffer[m_number++] =  t;}
  void  push(T**  t) noexcept{m_buffer[m_number++] = *t;}

  int  size() const noexcept{return m_number;}

  filtering_table&  reset() noexcept{  m_number = 0;  return *this;}

};




enum class
battle_result
{
  continuing,
  draw,
  left_won,
  right_won,

};


enum class
battle_action_kind
{
  null,
  commit,
  attack,
  avoid,
  receive,

};


class
battle_action
{
  battle_action_kind  m_kind;

  int  m_code;

  battles::character*   m_actor;
  battles::character*  m_target;

public:
  battle_action(battle_action_kind  k, int  c, battles::character&  actor, battles::character*  target=nullptr) noexcept:
  m_kind(k), m_code(c), m_actor(&actor), m_target(target){}

  battle_action_kind  get_kind() const noexcept{return m_kind;}

  int  get_code() const noexcept{return m_code;}

  battles::character&  get_actor()  const noexcept{return *m_actor;}
  battles::character*  get_target() const noexcept{return m_target;}

};


struct field_line;


enum class
intensity
{
  do_not,
  do_weakly,
  do_normally,
  do_strongly,
};


struct
piece
{
  struct flags{
    static constexpr int  be_pressed_from_left  = 1;
    static constexpr int  be_pressed_from_right = 2;
  };

  gbstd::status_value<int>  m_status;

  field_line*  m_line;

  int  m_y_position;

  intensity  m_offense_intensity;
  intensity  m_defense_intensity;
  intensity  m_movement_intensity;

  double  m_offense_power_base;
  double  m_defense_power_base;
  double  m_movement_power_base;

  double  m_offense_power;
  double  m_defense_power;
  double  m_movement_power;

  double  m_weight;

  double  m_previous_position;
  double  m_position;

  battles::side  m_body_direction;
  battles::side  m_move_direction;

  int  m_action_counter;

  gbstd::rectangle  m_image_rect;

  int  m_animation_index;
  int  m_animation_step_counter;
  int  m_animation_frame_counter;

  battles::side      m_side;
  battles::position  m_battle_position;

  piece*  m_friend;

  double  m_hp;

  double  minpos() const noexcept{return std::min(m_previous_position,m_position);}
  double  maxpos() const noexcept{return std::max(m_previous_position,m_position);}

};


struct
field_line
{
  int  m_y_position;

  piece  m_piece;

};


struct
battle_section
{
  static constexpr int  m_line_table_length = 20;

  field_line  m_line_table[m_line_table_length];

  int  m_number_of_lines=0;

  static constexpr int  m_table_length = 10;

  using character_table = filtering_table<battles::character,m_table_length>;

  gbstd::clock  m_clock;

  gbstd::task  m_task;


  gbstd::windows::stack  m_window_stack;

  gbstd::menus::table  m_first_menu_table;
  gbstd::menus::view   m_first_menu_view;

  gbstd::menus::table  m_second_menu_table;
  gbstd::menus::view   m_second_menu_view;

  gbstd::menus::table  m_third_menu_table;
  gbstd::menus::view   m_third_menu_view;


  battles::character  m_character_table[m_table_length];

  character_table  m_filtering_table;

  int  m_number_of_total_characters;

  int  m_number_of_left_characters;
  int  m_number_of_right_characters;

  int  m_number_of_playing_characters;

/*
  static void  manage_first_menu(menu_stack&  stk, const menu_result*  res, menu_view&  view, battle_section&  b) noexcept;
  static void  manage_handwork_menu(menu_stack&  stk, const menu_result*  res, menu_view&  view, battle_section&  b) noexcept;
  static void  manage_footwork_menu(menu_stack&  stk, const menu_result*  res, menu_view&  view, battle_section&  b) noexcept;
  static void  manage_third_menu(menu_stack&  stk, const menu_result*  res, menu_view&  view, battle_section&  b) noexcept;
*/

  void  initialize_menu() noexcept;

  static void  start_battle(gbstd::execution&  exec, battle_section&  b) noexcept;
  static void  loop_battle( gbstd::execution&  exec, battle_section&  b) noexcept;

  static void  initialize_battle(gbstd::execution&  exec, battle_section&  b) noexcept;
  static void         run_battle(gbstd::execution&  exec, battle_section&  b) noexcept;
  static void    finalize_battle(gbstd::execution&  exec, battle_section&  b) noexcept;

  static void  wait_until_end_movie(gbstd::execution&  exec, battle_section&  b) noexcept;

  battle_result  judge_battle() const noexcept;

  void  fight(gbstd::execution&  exec) noexcept;
  void  distribute_ap(int  v) noexcept;

  bool  judge_hit(const battles::character&  actor, const battles::character&  target) const noexcept;
  int   calculate_damage(const battles::character&  actor, const battles::character&  target) const noexcept;

  battles::character*  get_battle_character_by_ap() noexcept;


  battle_section(gbstd::execution&  exec) noexcept;
 ~battle_section();

  void  reset() noexcept;
  void  clear() noexcept;

  template<typename  TESTER>
  character_table&  filter(TESTER  t) noexcept{
    return m_filtering_table.assign(m_character_table).filter(t);
  }

  void  entry_party(const party&  p, battles::side  side) noexcept;

};


}




#endif




