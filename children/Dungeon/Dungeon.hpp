#ifndef Dungeon_HPP_WAS_INCLUDED_
#define Dungeon_HPP_WAS_INCLUDED_


#include"children/children.hpp"
#include<deque>
#include<memory>




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


class      node;
class     floor;
class structure;
class     world;


class
node
{
  floor*  m_floor=nullptr;

  gbstd::point  m_point;

  char  m_symbol;
  char  m_event_symbol;

public:
  node&  assign(floor&  fl, gbstd::point  pt, int  w, int  h) noexcept;

  bool  is_x_way()      const noexcept{return !(m_point.x&1);}
  bool  is_y_way()      const noexcept{return !(m_point.y&1);}
  bool  is_wall()       const noexcept{return is_soft_wall() || is_hard_wall();}
  bool  is_soft_wall()  const noexcept{return m_symbol == '+';}
  bool  is_hard_wall()  const noexcept{return m_symbol == '*';}
  bool  is_holed_wall() const noexcept{return m_symbol == '#';}
  bool  is_space()      const noexcept{return m_symbol == ' ';}
  bool  is_padding()    const noexcept{return m_symbol == 'O';}
  bool  is_up_way()     const noexcept{return m_symbol == 'U';}
  bool  is_down_way()   const noexcept{return m_symbol == 'D';}

  bool  is_passable() const noexcept{return is_space() || is_up_way() || is_down_way() || is_holed_wall();}

  char  get_symbol() const noexcept{return m_symbol;}

  int  get_event_code() const noexcept{return m_event_symbol-' ';}

  node&  be_soft_wall()  noexcept{  m_symbol = '+';  return *this;}
  node&  be_holed_wall() noexcept{  m_symbol = '#';  return *this;}
  node&  be_space()      noexcept{  m_symbol = ' ';  return *this;}
  node&  be_down_way()   noexcept{  m_symbol = 'D';  return *this;}
  node&  be_up_way()     noexcept{  m_symbol = 'U';  return *this;}

  void  print() const noexcept;
  void  print(std::string&  sbuf) const noexcept;

  void  scan(const char*&  s) noexcept;

};


class
floor
{
  structure*  m_structure=nullptr;

  int  m_number=0;

  std::string  m_name;

  static node  m_null_node;

  std::vector<node>  m_table;

public:
  floor(structure&  st, int  n, gbstd::values::array&  arr) noexcept;

  int  get_number() const noexcept{return m_number;}

  const std::string&  get_name() const noexcept{return m_name;}

  structure&  get_structure() const noexcept{return *m_structure;}

        node&  get_node(gbstd::point  pt)       noexcept;
  const node&  get_node(gbstd::point  pt) const noexcept;

  bool  test_passability(gbstd::point  pt) const noexcept;

  void  print() const noexcept;
  void  print(std::string&  sbuf) const noexcept;

};


class
entry
{
  std::string  m_label;

  gbstd::point  m_point;

  absolute_direction  m_direction;

public:
  entry() noexcept{}

  const auto&  get_label() const noexcept{return m_label;}

  const auto&  get_direction() const noexcept{return m_direction;}
  const auto&  get_point()     const noexcept{return m_point;}

};


class
structure
{
  std::string  m_name;

  int  m_width =0;
  int  m_height=0;

  std::vector<std::unique_ptr<floor>>  m_floors;

  std::vector<entry>  m_entries;

public:
  structure() noexcept{}
  structure(const gbstd::values::array&  arr) noexcept{scan(arr);}

  gbstd::rectangle  get_full_rect() const noexcept{return {0,0,m_width,m_height};}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int  get_number_of_floors() const noexcept{return m_floors.size();}

  floor&  operator[](int  i) const noexcept{return *m_floors[(i < 0)? 0:(i >= m_floors.size())? m_floors.size()-1:i];}

  const std::string&  get_name() const noexcept{return m_name;}

  void  print() const noexcept;
  void  print(std::string&  sbuf) const noexcept;

  void  scan(const gbstd::values::array&  arr) noexcept;

};


class
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

public:
  venturer&  operator+=(gbstd::point  pt)        noexcept{  m_point     +=  pt;  return *this;}
  venturer&  operator+=(relative_direction  dir) noexcept{  m_direction += dir;  return *this;}

  venturer&  set_current_floor(floor&  fl) noexcept{  m_floor = &fl;  return *this;}

  const gbstd::point&  get_point() const noexcept{return m_point;}

  venturer&  set_point(gbstd::point  pt) noexcept{  m_point = pt;  return *this;}

  const absolute_direction&  get_direction(                       ) const noexcept{return m_direction;}
  venturer&                  set_direction(absolute_direction  dir)       noexcept{       m_direction = dir;  return *this;}

  floor&  get_current_floor() const noexcept{return *m_floor;}
  node&   get_current_node() const noexcept{return  m_floor->get_node(m_point);}

  void  draw_status(const gbstd::canvas&  cv) noexcept;
  void  draw_around(const gbstd::canvas&  cv) noexcept;

  std::string_view  get_facility_name() const noexcept;

  bool  test_advance() const noexcept;
  void       advance()       noexcept;

  void  move_above() noexcept;
  void  move_below() noexcept;

};


struct
menu
{
  gbstd::window  m_window;

  std::vector<std::u16string>  m_strings;

  int  m_index;

  void  reset(std::initializer_list<std::u16string_view>  ls) noexcept;

};


class
world
{
  venturer  m_venturer;

  std::vector<std::unique_ptr<structure>>   m_structures;

  std::deque<char16_t>  m_character_buffer;

  gbstd::text  m_text;

  gbstd::alarm  m_text_alarm;

  gbstd::program_space      m_prosp;
  gbstd::program_execution  m_proex;

  bool  m_rapid_stream=false;
  bool  m_stream_finished=false;

  menu  m_menu;

  gbstd::gpfs::node_reference  m_base_sprite;
  gbstd::gpfs::node_reference  m_text_sprite;
  gbstd::gpfs::node_reference  m_menu_sprite;

  void  push_text(std::string     sv) noexcept;
  void  push_text(std::u16string  sv) noexcept;

  void  undisplay_text() noexcept;
  void  display_text1() noexcept;
  void  display_text2() noexcept;

  bool  stream_text() noexcept;

  void  execute_program(gbstd::execution&  exec) noexcept;

  void  wait_input(gbstd::execution&  exec) noexcept;

  void  initialize(gbstd::execution&  exec) noexcept;

  void  start_creation_menu(gbstd::execution&  exec) noexcept;

  void  finish_event(gbstd::execution&  exec) noexcept;


  void  draw_walls1(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;
  void  draw_walls2(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;
  void  draw_walls3(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;

  void  draw_base(const gbstd::canvas&  cv) noexcept;
  void  draw_text(const gbstd::canvas&  cv) noexcept;
  void  draw_creation_menu(const gbstd::canvas&  cv) noexcept;

  void  print() const noexcept;
  void  print(std::string&  sbuf) const noexcept;

  void  scan(std::string_view  sv) noexcept;

public:
  world() noexcept{push_structure();}

  structure&  push_structure() noexcept;
  void         pop_structure() noexcept{m_structures.pop_back();}

  static game_information  get_information() noexcept;

};


}


#endif




