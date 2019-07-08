#ifndef ControlDevice_hpp_was_included
#define ControlDevice_hpp_was_included


#include<cstdint>
#include<cstdio>
#include<vector>
#include"libgbstd/utility.hpp"


namespace gbstd{




template<typename  T>
struct
basic_point
{
  T  x;
  T  y;

  constexpr basic_point(T  x_=0, T  y_=0) noexcept:
  x(x_),
  y(y_){}

  template<typename  U>
  constexpr basic_point(basic_point<U>  rhs) noexcept:
  x(rhs.x),
  y(rhs.y){}

  template<typename  U>
  constexpr basic_point&  operator=(basic_point<U>  rhs) noexcept
  {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);

    return *this;
  }


  constexpr bool  operator==(basic_point  rhs) const noexcept
  {
    return(x == rhs.x) &&
          (y == rhs.y);
  }

  constexpr bool  operator!=(basic_point  rhs) const noexcept
  {
    return(x != rhs.x) ||
          (y != rhs.y);
  }

  constexpr basic_point  operator+(basic_point  rhs) const noexcept{return basic_point(x+rhs.x,y+rhs.y);}
  constexpr basic_point  operator-(basic_point  rhs) const noexcept{return basic_point(x-rhs.x,y-rhs.y);}
  constexpr basic_point  operator*(basic_point  rhs) const noexcept{return basic_point(x*rhs.x,y*rhs.y);}
  constexpr basic_point  operator/(basic_point  rhs) const noexcept{return basic_point(x/rhs.x,y/rhs.y);}
  constexpr basic_point  operator%(basic_point  rhs) const noexcept{return basic_point(x%rhs.x,y%rhs.y);}
  constexpr basic_point  operator-() const noexcept{return basic_point(-x,-y);}

  basic_point&  operator+=(basic_point  rhs) noexcept{  x += rhs.x;  y += rhs.y;  return *this;}
  basic_point&  operator-=(basic_point  rhs) noexcept{  x -= rhs.x;  y -= rhs.y;  return *this;}
  basic_point&  operator*=(basic_point  rhs) noexcept{  x *= rhs.x;  y *= rhs.y;  return *this;}
  basic_point&  operator/=(basic_point  rhs) noexcept{  x /= rhs.x;  y /= rhs.y;  return *this;}
  basic_point&  operator%=(basic_point  rhs) noexcept{  x %= rhs.x;  y %= rhs.y;  return *this;}

  void  print() const noexcept{printf("{x:%3d,y:%3d}",static_cast<int>(x),static_cast<int>(y));}

};


using point = basic_point<int>;


struct
rectangle
{
  int  x;
  int  y;
  int  w;
  int  h;

  static constexpr int  abs(int  i) noexcept{return (i < 0)? -i:i;}

  constexpr rectangle(int  x_=0, int  y_=0, int  w_=0, int  h_=0) noexcept:
  x(x_), y(y_), w(w_), h(h_){}

  constexpr rectangle(point  pt, int  w_=0, int  h_=0) noexcept:
  x(pt.x), y(pt.y), w(w_), h(h_){}

  constexpr rectangle(point  a, point  b) noexcept:
  x(std::min(a.x,b.x)), y(std::min(a.y,b.y)), w(1+abs(a.x-b.x)), h(1+abs(a.y-b.y)){}

  constexpr bool  test_point(const point&  pt) const noexcept
  {
    return((pt.x >= (x  )) &&
           (pt.y >= (y  )) &&
           (pt.x <  (x+w)) &&
           (pt.y <  (y+h)));
  }


  constexpr int  get_x()     const noexcept{return x  ;}
  constexpr int  get_x_end() const noexcept{return x+w;}
  constexpr int  get_y()     const noexcept{return y  ;}
  constexpr int  get_y_end() const noexcept{return y+h;}

  constexpr int  get_w() const noexcept{return w;}
  constexpr int  get_h() const noexcept{return h;}

  static constexpr int  get_left(const rectangle&  a, const rectangle&  b) noexcept
  {
    return (a.x < b.x)? b.x:a.x;
 }


  static constexpr int  get_top(const rectangle&  a, const rectangle&  b) noexcept
  {
    return (a.y < b.y)? b.y:a.y;
  }


  static constexpr int  get_right(const rectangle&  a, const rectangle&  b) noexcept
  {
    return (a.get_x_end() < b.get_x_end())? a.get_x_end():b.get_x_end();
  }


  static constexpr int  get_bottom(const rectangle&  a, const rectangle&  b) noexcept
  {
    return (a.get_y_end() < b.get_y_end())? a.get_y_end():b.get_y_end();
  }


  static constexpr rectangle  get_overlap(const rectangle&  a, const rectangle&  b) noexcept
  {
    return rectangle(get_left(a,b),
                     get_top(a,b),
                     get_right(a,b)-get_left(a,b)+1,
                     get_bottom(a,b)-get_top(a,b)+1);
  }

  constexpr rectangle  operator&(const rectangle&  rhs) const noexcept
  {
    return get_overlap(*this,rhs);
  }

  rectangle&  operator&=(const rectangle&  rhs) noexcept
  {
           *this = (*this)&rhs;
    return *this              ;
  }

  void  print() const noexcept{printf("{x:%3d,y:%3d,w:%3d,h:%3d}",x,y,w,h);}

};




struct
area
{
  int     top=0;
  int    left=0;
  int   right=0;
  int  bottom=0;


  static constexpr bool  test_x_collision(const area&  a, const area&  b) noexcept
  {
      if(a.left < b.left){return a.right >= b.left;}
    else                 {return b.right >= a.left;}
  }


  static constexpr bool  test_y_collision(const area&  a, const area&  b) noexcept
  {
      if(a.top < b.top){return a.bottom >= b.top;}
    else               {return b.bottom >= a.top;}
  }


  static constexpr bool  test_collision(const area&  a, const area&  b) noexcept
  {
    return(test_x_collision(a,b) &&
           test_y_collision(a,b));
  }

  void  print() const noexcept
  {
    printf("area:{top:%4d,left:%4d,right:%4d,bottom:%4d}\n",top,left,right,bottom);
  }

};




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




