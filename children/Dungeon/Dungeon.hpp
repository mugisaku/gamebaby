#ifndef Dungeon_HPP_WAS_INCLUDED_
#define Dungeon_HPP_WAS_INCLUDED_


#include"children/children.hpp"
#include<deque>




namespace Dungeon{


class
relative_direction
{
  int  m_value;

public:
  constexpr relative_direction(int  v=0) noexcept: m_value(v){}

  constexpr bool  operator==(relative_direction  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(relative_direction  rhs) const noexcept{return m_value != rhs.m_value;}

  constexpr bool  is_front() const noexcept{return m_value == 0;}
  constexpr bool  is_right() const noexcept{return m_value == 1;}
  constexpr bool  is_back()  const noexcept{return m_value == 2;}
  constexpr bool  is_left()  const noexcept{return m_value == 3;}

  constexpr int  get_value() const noexcept{return m_value;}

};


class
absolute_direction
{
  int  m_value;

public:
  constexpr absolute_direction(int  v=0) noexcept: m_value(v){}

  constexpr bool  operator==(absolute_direction  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(absolute_direction  rhs) const noexcept{return m_value != rhs.m_value;}

  constexpr bool  is_x_positive() const noexcept{return m_value == 0;}
  constexpr bool  is_y_positive() const noexcept{return m_value == 1;}
  constexpr bool  is_x_negative() const noexcept{return m_value == 2;}
  constexpr bool  is_y_negative() const noexcept{return m_value == 3;}

  constexpr operator int() const noexcept{return m_value;}

  constexpr int  get_value() const noexcept{return m_value;}

  constexpr absolute_direction  operator+(relative_direction  reldir) const noexcept{return {(m_value+reldir.get_value())&3};}
  constexpr absolute_direction  operator-(relative_direction  reldir) const noexcept{return {(m_value-reldir.get_value())&3};}

  absolute_direction&  operator+=(relative_direction  reldir) noexcept{  m_value = ((m_value+reldir.get_value())&3);  return *this;}
  absolute_direction&  operator-=(relative_direction  reldir) noexcept{  m_value = ((m_value-reldir.get_value())&3);  return *this;}

};


constexpr gbstd::point
transform(const gbstd::point&  pt, absolute_direction  dir) noexcept
{
  return dir.is_y_positive()? pt
        :dir.is_x_positive()? gbstd::point( pt.y,-pt.x)
        :dir.is_y_negative()? gbstd::point(-pt.x,-pt.y)
        :dir.is_x_negative()? gbstd::point(-pt.y, pt.x)
        :pt;
}


constexpr gbstd::point
transform(const gbstd::point&  pt, relative_direction  dir) noexcept
{
  return dir.is_front()? pt
        :dir.is_right()? gbstd::point( pt.y,-pt.x)
        :dir.is_back()?  gbstd::point(-pt.x,-pt.y)
        :dir.is_left()?  gbstd::point(-pt.y, pt.x)
        :pt;
}


constexpr int  g_number_of_directions = 4;


namespace directions{
constexpr absolute_direction  x_positive = absolute_direction(0);
constexpr absolute_direction  y_positive = absolute_direction(1);
constexpr absolute_direction  x_negative = absolute_direction(2);
constexpr absolute_direction  y_negative = absolute_direction(3);

constexpr relative_direction  front = relative_direction(0);
constexpr relative_direction  right = relative_direction(1);
constexpr relative_direction   back = relative_direction(2);
constexpr relative_direction   left = relative_direction(3);
}


class     space;
class     floor;
class structure;
class     world;


struct
node
{
  floor*  m_floor=nullptr;

  gbstd::point  m_point;

  char  m_symbol;
  char  m_lock;

  char  m_wayhole;

  std::string_view  m_event_code;

  bool  is_x_way() const noexcept{return !(m_point.x&1);}
  bool  is_y_way() const noexcept{return !(m_point.y&1);}
  bool  is_wall()  const noexcept{return m_symbol == '+';}
  bool  is_space() const noexcept{return m_symbol == ' ';}

  bool  is_passable() const noexcept{return is_space() || has_wayhole();}

  bool  has_wayhole() const noexcept{return m_wayhole;}

  node&  be_wall()  noexcept{  m_symbol = '+';                      return *this;}
  node&  be_space() noexcept{  m_symbol = ' ';  m_wayhole = false;  return *this;}

  node&    hold_wayhole() noexcept{  m_wayhole =  true;  return *this;}
  node&  unhold_wayhole() noexcept{  m_wayhole = false;  return *this;}

};


struct
floor
{
  structure*  m_structure=nullptr;

  static constexpr int  m_width  = 33;
  static constexpr int  m_height = 33;
  static constexpr gbstd::rectangle  m_full_rect = {0,0,m_width,m_height};

  int  m_number=0;

  static node  m_null_node;

  node  m_table[m_height][m_width];

  void  reset(structure&  st, int  n) noexcept;

  node&  get_node(gbstd::point  pt) noexcept{return m_full_rect.test_point(pt)? m_table[pt.y][pt.x]:m_null_node;}

  bool  test_passability(gbstd::point  pt) const noexcept{return m_table[pt.y][pt.x].is_passable();}

  void  print() const noexcept;

};


struct
structure
{
  static constexpr int  m_number_of_floors = 10;

  std::string  m_name;

  floor  m_floors[m_number_of_floors];

  void  reset() noexcept;

};


struct
venturer
{
  floor*  m_floor=nullptr;

  int  m_hp_max=30;
  int  m_hp    =30;

  int  m_strength=10;
  int  m_defense =10;
  int  m_agility =10;
  int  m_luck    =10;

  gbstd::point  m_point;

  absolute_direction  m_direction;

  floor&  get_current_floor() const noexcept{return *m_floor;}
  node&   get_current_node() const noexcept{return  m_floor->get_node(m_point);}

  void  draw_status(const gbstd::canvas&  cv) noexcept;
  void  draw_around(const gbstd::canvas&  cv) noexcept;

  bool  test_advance() const noexcept;
  void       advance()       noexcept;

};


struct
world
{
  venturer    m_venturer;
  structure   m_structure;

  std::deque<char16_t>  m_character_buffer;

  gbstd::text  m_text;

  gbstd::alarm  m_text_alarm;

  gbstd::program_space      m_prosp;
  gbstd::program_execution  m_proex;

  bool  m_rapid_stream=false;
  bool  m_stream_finished=false;

  gbstd::gpfs::node_reference  m_base_sprite;
  gbstd::gpfs::node_reference  m_text_sprite;

  void  push_text(std::string     sv) noexcept;
  void  push_text(std::u16string  sv) noexcept;

  void  undisplay_text() noexcept;
  void  display_text1() noexcept;
  void  display_text2() noexcept;

  bool  stream_text() noexcept;

  void  execute_program(gbstd::execution&  exec) noexcept;

  void  wait_input(gbstd::execution&  exec) noexcept;

  void  initialize(gbstd::execution&  exec) noexcept;

  void  finish_event(gbstd::execution&  exec) noexcept;


  void  draw_walls1(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;
  void  draw_walls2(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;
  void  draw_walls3(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;

  void  draw_base(const gbstd::canvas&  cv) noexcept;
  void  draw_text(const gbstd::canvas&  cv) noexcept;

public:

  static game_information  get_information() noexcept;

};


}


#endif




