#ifndef ww_bar_hpp_is_included
#define ww_bar_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libgbstd/instruction_queue.hpp"
#include<list>
#include<vector>
#include<functional>


namespace ww{




class
bar
{
public:
  enum class mode{
    left_to_right,
    right_to_left,
  };

private:
  gbstd::fixed_t  m_desired_length=0;
  gbstd::fixed_t  m_length=0;
  gbstd::fixed_t  m_increment=0;

  gbstd::real_point  m_pos;

  uint32_t  m_time_to_reach=0;

  int  m_thickness=0;

  gbstd::color  m_color=gbstd::colors::white;

  mode  m_mode=mode::left_to_right;

public:
  bar() noexcept{}

  bool  is_left_to_right() const noexcept{return m_mode == mode::left_to_right;}
  bool  is_right_to_left() const noexcept{return m_mode == mode::right_to_left;}

  bar&  set_position(int  x, int  y)   noexcept{  m_pos              = {x,y};  return *this;}
  bar&  set_thickness(int  v)          noexcept{  m_thickness        =     v;  return *this;}
  bar&  set_mode(bar::mode  mode)      noexcept{  m_mode             =  mode;  return *this;}
  bar&  set_length(int  v)             noexcept{  m_length           =     v;  return *this;}
  bar&  set_desired_length(int  v)     noexcept{  m_desired_length   =     v;  return *this;}
  bar&  set_color(gbstd::color  color) noexcept{  m_color            = color;  return *this;}
  bar&  set_time_to_reach(uint32_t  t) noexcept{  m_time_to_reach    =     t;  return *this;}

  uint32_t  get_time_to_reach() const noexcept{return m_time_to_reach;}

  int  get_thickness() const noexcept{return m_thickness;}

  gbstd::fixed_t  get_length()         const noexcept{return m_length;}
  gbstd::fixed_t  get_desired_length() const noexcept{return m_desired_length;}
  gbstd::fixed_t  get_increment()      const noexcept{return m_increment;}
  gbstd::color    get_color()      const noexcept{return m_color;}

  bool  is_finished() const noexcept{return m_length == m_desired_length;}

  void  update_increment() noexcept;

  static void  draw(const gbstd::canvas&  cv, bar&  b) noexcept;
  static void  tick(                          bar&  b) noexcept;

};


}




#endif




