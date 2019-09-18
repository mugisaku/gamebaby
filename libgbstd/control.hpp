#ifndef ControlDevice_hpp_was_included
#define ControlDevice_hpp_was_included


#include<cstdint>
#include<cstdio>
#include<vector>
#include"libgbstd/image.hpp"


namespace gbstd{




class
user_input
{
  int  m_state;

  struct flags{
    static constexpr int     up = 0x001;
    static constexpr int   left = 0x002;
    static constexpr int  right = 0x004;
    static constexpr int   down = 0x008;
    static constexpr int  shift = 0x010;
    static constexpr int  start = 0x020;
    static constexpr int      p = 0x100;
    static constexpr int      n = 0x200;
    static constexpr int      l = 0x400;
    static constexpr int      r = 0x800;

    static constexpr int  mouse_left  = 0x1000;
    static constexpr int  mouse_right = 0x2000;
  };

  bool   test(int  flag) const noexcept{return m_state&flag;}

  void    set(int  flag) noexcept{m_state |=  flag;}
  void  unset(int  flag) noexcept{m_state &= ~flag;}

public:
  user_input(int  state=0) noexcept: m_state(state){}

  user_input  operator^(user_input  rhs) const noexcept{return user_input(m_state^rhs.m_state);}

  operator bool() const noexcept{return m_state;}

  void  clear() noexcept{m_state = 0;}

  int  get_code() const noexcept{return m_state;}

  bool    test_all() const noexcept{return m_state;}
  bool     test_up() const noexcept{return test(flags::up);}
  bool   test_left() const noexcept{return test(flags::left);}
  bool  test_right() const noexcept{return test(flags::right);}
  bool   test_down() const noexcept{return test(flags::down);}

  bool   test_mouse_left() const noexcept{return test(flags::mouse_left);}
  bool  test_mouse_right() const noexcept{return test(flags::mouse_right);}

  bool  test_p() const noexcept{return test(flags::p);}
  bool  test_n() const noexcept{return test(flags::n);}
  bool  test_l() const noexcept{return test(flags::l);}
  bool  test_r() const noexcept{return test(flags::r);}

  bool  test_shift() const noexcept{return test(flags::shift);}
  bool  test_start() const noexcept{return test(flags::start);}


  void     set_up() noexcept{set(flags::up);}
  void   set_left() noexcept{set(flags::left);}
  void  set_right() noexcept{set(flags::right);}
  void   set_down() noexcept{set(flags::down);}

  void   set_mouse_left() noexcept{set(flags::mouse_left);}
  void  set_mouse_right() noexcept{set(flags::mouse_right);}

  void  set_p() noexcept{set(flags::p);}
  void  set_n() noexcept{set(flags::n);}

  void  set_l() noexcept{set(flags::l);}
  void  set_r() noexcept{set(flags::r);}

  void  set_shift() noexcept{set(flags::shift);}
  void  set_start() noexcept{set(flags::start);}


  void     unset_up() noexcept{unset(flags::up);}
  void   unset_left() noexcept{unset(flags::left);}
  void  unset_right() noexcept{unset(flags::right);}
  void   unset_down() noexcept{unset(flags::down);}

  void   unset_mouse_left() noexcept{unset(flags::mouse_left);}
  void  unset_mouse_right() noexcept{unset(flags::mouse_right);}

  void  unset_p() noexcept{unset(flags::p);}
  void  unset_n() noexcept{unset(flags::n);}
  void  unset_l() noexcept{unset(flags::l);}
  void  unset_r() noexcept{unset(flags::r);}

  void  unset_shift() noexcept{unset(flags::shift);}
  void  unset_start() noexcept{unset(flags::start);}

};


extern uint32_t  g_time;

extern std::vector<point>  g_point_buffer;

extern std::vector<uint8_t>  g_dropped_file;

extern bool  g_needed_to_redraw;

extern user_input  g_modified_input;
extern user_input           g_input;

void  barrier_input(uint32_t  interval=240) noexcept;
bool  test_input_barrier() noexcept;

bool  is_user_time_flowing() noexcept;

void  start_user_time() noexcept;
void   stop_user_time() noexcept;

uint32_t    get_user_time() noexcept;
uint32_t  get_system_time() noexcept;


void  add_virtual_time(uint32_t  ms) noexcept;

uint32_t  get_virtual_time() noexcept;

void  print_time_status() noexcept;


}


#endif




