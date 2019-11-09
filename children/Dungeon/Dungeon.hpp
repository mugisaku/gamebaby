#ifndef Dungeon_HPP_WAS_INCLUDED_
#define Dungeon_HPP_WAS_INCLUDED_


#include"children/children.hpp"




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
class structure;


struct
wall
{
  space*  m_space;

  bool  m_passage;

  int  m_event_index;

};


struct
space
{
  structure*  m_structure;

  gbstd::point  m_point;

  bool  m_block=false;

  wall  m_walls[g_number_of_directions];

};


struct
structure
{
  static constexpr int  m_width  = 32;
  static constexpr int  m_height = 32;

  space  m_table[m_height][m_width];

  void  reset() noexcept;

};


struct
venturer
{
  structure*  m_structure;

  gbstd::point  m_point;

  absolute_direction  m_direction;

};


struct
world
{
  venturer    m_venturer;
  structure   m_structure;

  bool  is_block(gbstd::point  pt) const noexcept;

  void  wait_input(gbstd::execution&  exec) noexcept;

  void  initialize(gbstd::execution&  exec) noexcept;

  void  draw_around(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;
  void  draw_walls(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept;

  void  draw(const gbstd::canvas&  cv) noexcept;

  static game_information  get_information() noexcept;

};


}


#endif




