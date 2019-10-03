#ifndef GBSTD_Control_hpp_was_included
#define GBSTD_Control_hpp_was_included


#include<cstdint>
#include<cstdio>
#include<vector>
#include"libgbstd/image.hpp"
#include"libgbstd/misc.hpp"


namespace gbstd{




class
key_state
{
  int  m_value;

  struct flags{
    static constexpr int  diu = 0x001;
    static constexpr int  dil = 0x002;
    static constexpr int  dir = 0x004;
    static constexpr int  did = 0x008;
    static constexpr int  acu = 0x010;
    static constexpr int  acl = 0x020;
    static constexpr int  acr = 0x040;
    static constexpr int  acd = 0x080;
    static constexpr int  opl = 0x100;
    static constexpr int  opr = 0x200;
    static constexpr int  shl = 0x400;
    static constexpr int  shr = 0x800;

    static constexpr int  mol = 0x1000;
    static constexpr int  mor = 0x2000;

  };

  bool   test(int  flag) const noexcept{return m_value&flag;}

  key_state&    set(int  flag) noexcept{  m_value |=  flag;  return *this;}
  key_state&  unset(int  flag) noexcept{  m_value &= ~flag;  return *this;}

public:
  key_state(int  v=0) noexcept: m_value(v){}

  key_state  operator^(key_state  rhs) const noexcept{return key_state(m_value^rhs.m_value);}

  operator bool() const noexcept{return m_value;}

  void  clear() noexcept{m_value = 0;}

  int  get_value() const noexcept{return m_value;}

  bool  test_acu() const noexcept{return test(flags::acu);}
  bool  test_acl() const noexcept{return test(flags::acl);}
  bool  test_acr() const noexcept{return test(flags::acr);}
  bool  test_acd() const noexcept{return test(flags::acd);}

  bool  test_diu() const noexcept{return test(flags::diu);}
  bool  test_dil() const noexcept{return test(flags::dil);}
  bool  test_dir() const noexcept{return test(flags::dir);}
  bool  test_did() const noexcept{return test(flags::did);}

  bool  test_opl() const noexcept{return test(flags::opl);}
  bool  test_opr() const noexcept{return test(flags::opr);}
  bool  test_shl() const noexcept{return test(flags::shl);}
  bool  test_shr() const noexcept{return test(flags::shr);}

  bool  test_mol() const noexcept{return test(flags::mol);}
  bool  test_mor() const noexcept{return test(flags::mor);}


  key_state&  set_diu() noexcept{return set(flags::diu);}
  key_state&  set_dil() noexcept{return set(flags::dil);}
  key_state&  set_dir() noexcept{return set(flags::dir);}
  key_state&  set_did() noexcept{return set(flags::did);}

  key_state&  set_acu() noexcept{return set(flags::acu);}
  key_state&  set_acl() noexcept{return set(flags::acl);}
  key_state&  set_acr() noexcept{return set(flags::acr);}
  key_state&  set_acd() noexcept{return set(flags::acd);}

  key_state&  set_opl() noexcept{return set(flags::opl);}
  key_state&  set_opr() noexcept{return set(flags::opr);}
  key_state&  set_shl() noexcept{return set(flags::shl);}
  key_state&  set_shr() noexcept{return set(flags::shr);}

  key_state&  set_mol() noexcept{return set(flags::mol);}
  key_state&  set_mor() noexcept{return set(flags::mor);}


  key_state&  unset_diu() noexcept{return unset(flags::diu);}
  key_state&  unset_dil() noexcept{return unset(flags::dil);}
  key_state&  unset_dir() noexcept{return unset(flags::dir);}
  key_state&  unset_did() noexcept{return unset(flags::did);}

  key_state&  unset_acu() noexcept{return unset(flags::acu);}
  key_state&  unset_acl() noexcept{return unset(flags::acl);}
  key_state&  unset_acr() noexcept{return unset(flags::acr);}
  key_state&  unset_acd() noexcept{return unset(flags::acd);}

  key_state&  unset_opl() noexcept{return unset(flags::opl);}
  key_state&  unset_opr() noexcept{return unset(flags::opr);}
  key_state&  unset_shl() noexcept{return unset(flags::shl);}
  key_state&  unset_shr() noexcept{return unset(flags::shr);}

  key_state&  unset_mol() noexcept{return unset(flags::mol);}
  key_state&  unset_mor() noexcept{return unset(flags::mor);}

};




class
clock
{
  status_value<int>  m_status;

  struct flags{
    static constexpr int    stop = 1;
    static constexpr int   pause = 2;
  };

  int  m_permil=1000;

  uint32_t  m_time_integer=0;
  uint32_t  m_time_fraction=0;

public:
  operator bool() const noexcept{return !m_status;}

  bool   is_stopped() const noexcept{return m_status.test(flags::stop);}
  bool    is_paused() const noexcept{return m_status.test(flags::pause);}

  clock&  start() noexcept{  m_status.unset(flags::stop);  return *this;}
  clock&   stop() noexcept{  m_status.set(  flags::stop);  return *this;}

  clock&    pause() noexcept{  m_status.set(  flags::pause);  return *this;}
  clock&  unpause() noexcept{  m_status.unset(flags::pause);  return *this;}

  const int&  get_permil(      ) const noexcept{return m_permil;}
  clock&      set_permil(int  v)       noexcept{  m_permil = v;  return *this;}

  const uint32_t&  get_time() const noexcept{return m_time_integer;}

  clock&  operator>>=(uint32_t  t) noexcept;

};


class
timer
{
  status_value<int>  m_status;

  struct flags{
    static constexpr int    stop = 1;
    static constexpr int   pause = 2;
    static constexpr int    lock = 4;
  };


  const clock*  m_clock_pointer=nullptr;

  uint32_t  m_interval=1000;
  uint32_t  m_next_time=0;

  callback_wrapper  m_callback;

public:
  timer() noexcept{}
  timer(const clock&  clk, uint32_t  interval, callback_wrapper  cb) noexcept:
  m_clock_pointer(&clk), m_interval(interval), m_callback(cb){}

  bool  is_stopped() const noexcept{return m_status.test(flags::stop);}
  bool   is_paused() const noexcept{return m_status.test(flags::pause);}

  timer&  start() noexcept{  m_status.unset(flags::stop);  return *this;}
  timer&   stop() noexcept{  m_status.set(  flags::stop);  return *this;}

  timer&    pause() noexcept{  m_status.set(  flags::pause);  return *this;}
  timer&  unpause() noexcept{  m_status.unset(flags::pause);  return *this;}

  const uint32_t&  get_interval(           ) const noexcept{return m_interval;}
  timer&           set_interval(uint32_t  v)       noexcept{  m_interval = v;  return *this;}

  const clock*  get_clock_pointer(                 ) const noexcept{return m_clock_pointer;}
  timer&        set_clock_pointer(const clock&  clk)       noexcept{  m_clock_pointer = &clk;  return *this;}

  timer&  set_callback(callback_wrapper  cb) noexcept{  m_callback = cb;  return *this;}

  timer&  operator()() noexcept;

};


class
auto_counter: public timer
{
  int  m_value    =0;
  int  m_increment=1;

public:
  const int&  get_value(      ) const noexcept{return m_value;}
  timer&      set_value(int  v)       noexcept{  m_value = v;  return *this;}

  const int&  get_increment(      ) const noexcept{return m_increment;}
  timer&      set_increment(int  v)       noexcept{  m_increment = v;  return *this;}

  void  drive() noexcept{m_value += m_increment;}

};




void  push_execution(callback_wrapper  cb) noexcept;
void   pop_execution(int  v=0) noexcept;
void  step_execution() noexcept;

int  get_control_value() noexcept;

void  interrupt_execution() noexcept;


uint32_t     get_time(           ) noexcept;
void      update_time(uint32_t  t) noexcept;


void  allocate_clocks(int  n) noexcept;
clock&  get_clock(int  i) noexcept;


void  allocate_timers(int  n) noexcept;
timer&  get_timer(int  i) noexcept;


const key_state&  get_modified_keys(                      ) noexcept;
const key_state&   get_pressed_keys(                      ) noexcept;
const key_state&           get_keys(                      ) noexcept;
void                    update_keys(const key_state&  keys) noexcept;

void  barrier_keys(uint32_t  interval=240) noexcept;

void   update_point(point  pt) noexcept;
liner  make_liner() noexcept;

extern std::vector<uint8_t>  g_dropped_file;



}


#endif




