#ifndef FallBlockPazzle_HPP_WAS_INCLUDED_
#define FallBlockPazzle_HPP_WAS_INCLUDED_


#include"children/children.hpp"




namespace FallBlockPazzle{




struct
space
{
  friend class stage;

public:
  struct flags{
    static constexpr int  right_erase = 1;
    static constexpr int   down_erase = 2;
    static constexpr int        erase = right_erase|down_erase;

  };

private:
  static constexpr int  m_size = 16;

  gbstd::point  m_point;

  gbstd::status_value<int>  m_status;

  int  m_number=0;

  gbstd::color  m_color;

  space*     m_up;
  space*   m_left;
  space*  m_right;
  space*   m_down;

public:
  operator bool() const noexcept{return m_number;}

  bool     test_flag(int  flag) const noexcept{return m_status.test(flag);}
  space&    set_flag(int  flag) noexcept{  m_status.set(  flag);  return *this;}
  space&  unset_flag(int  flag) noexcept{  m_status.unset(flag);  return *this;}

  space*  get_up()    const noexcept{return    m_up;}
  space*  get_left()  const noexcept{return  m_left;}
  space*  get_right() const noexcept{return m_right;}
  space*  get_down()  const noexcept{return  m_down;}

  space&  set_number(int  n)       noexcept{       m_number = n;  return *this;}
  int     get_number(      ) const noexcept{return m_number;}

  space&        set_color(gbstd::color  c)       noexcept{       m_color = c;  return *this;}
  gbstd::color  get_color(               ) const noexcept{return m_color;}

  void  draw(const gbstd::canvas&  cv, int  y_offset, int  state) const noexcept;

};




struct
stage
{
  static constexpr int  m_score_base = 20;

  static constexpr int  m_key_number = 6;

  static constexpr int   m_width =  8;
  static constexpr int  m_height = 12;

  struct flags{
    static constexpr int     erase = 1;
    static constexpr int     chink = 2;
    static constexpr int  gameover = 4;
  };


  gbstd::uniform_rand  m_number_rand;
  gbstd::uniform_rand  m_color_rand;

  int  m_blink_counter=0;

  gbstd::status_value<int>  m_status;

  int  m_chink_counter=0;

  uint32_t  m_temporary_score=0;
  uint32_t  m_total_score=0;

  gbstd::alarm  m_alarm;

  space  m_table[m_height][m_width];

  space*  m_current=&m_table[0][0];

  stage() noexcept;

  void  reset() noexcept;

  void  evaluate_to(space*  head, const space*  end, int  flag, int  increment) noexcept;

  void  control_by_cursor(gbstd::execution&  exec, space*  next) noexcept;
  void  control_drop(gbstd::execution&  exec) noexcept;

  void  chink(gbstd::execution&  exec) noexcept;
  void  blink(gbstd::execution&  exec) noexcept;
  void  start(gbstd::execution&  exec) noexcept;
  void  evaluate(gbstd::execution&  exec) noexcept;
  void  gameover(gbstd::execution&  exec) noexcept;

  void  initialize(gbstd::execution&  exec) noexcept;

  void  add_temporary_score(int  v) noexcept{m_temporary_score += v*m_chink_counter*m_chink_counter;}

  void  draw(const gbstd::canvas&  cv) noexcept;

  static game_information  get_information() noexcept;

};


}


#endif




