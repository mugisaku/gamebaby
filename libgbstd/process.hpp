#ifndef gbstd_process_HPP
#define gbstd_process_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/image.hpp"
#include<utility>
#include<string_view>
#include<list>
#include<string>
#include<vector>
#include<cstdint>


namespace gbstd{




class
clock
{
  std::string  m_name;
  std::string    m_id;

  int  m_permil=1000;

  uint32_t  m_time=0;
  uint32_t  m_fraction=0;

  status_value<int>  m_status;

  struct flags{
    static constexpr int  working = 1;
    static constexpr int  pausing = 2;
  };

  friend class clock_master;

public:
  clock() noexcept{}

  clock&  clear() noexcept;
  clock&  reset(uint32_t  time=0, uint32_t  fraction=0) noexcept;

  clock&  add(uint32_t  t) noexcept;

  operator bool() const noexcept{return m_status.test(flags::working) && !m_status.test(flags::pausing);}

  clock&  set_name(std::string_view  sv) noexcept{  m_name = sv;  return *this;}
  clock&    set_id(std::string_view  sv) noexcept{  m_id   = sv;  return *this;}

  const std::string&  get_name() const noexcept{return m_name;}
  const std::string&  get_id()   const noexcept{return m_id;}

  const uint32_t&   get_time() const noexcept{return m_time;}

  bool  is_working() const noexcept{return m_status.test(flags::working);}
  bool  is_pausing() const noexcept{return m_status.test(flags::pausing);}

  clock&   turnon() noexcept{  m_status.set(  flags::working);  return *this;}
  clock&  turnoff() noexcept{  m_status.unset(flags::working);  return *this;}

  clock&    pause() noexcept{  m_status.set(  flags::pausing);  return *this;}
  clock&  unpause() noexcept{  m_status.unset(flags::pausing);  return *this;}

  int     get_permil(      ) const noexcept{return m_permil;}
  clock&  set_permil(int  v)       noexcept{  m_permil = v;  return *this;}

};


class
clock_watch
{
  static const clock  m_null;

  const clock*  m_pointer=&m_null;

public:
  clock_watch() noexcept{}
  clock_watch(const clock&  ref) noexcept: m_pointer(&ref){}

  const std::string&  get_name() const noexcept{return m_pointer->get_name();}
  const std::string&  get_id()   const noexcept{return m_pointer->get_id();}

  const uint32_t&   get_time() const noexcept{return m_pointer->get_time();}

};




class
task
{
  std::string  m_name;
  std::string  m_id;

  void  (*m_draw_callback)(const canvas&,dummy&)=nullptr;
  void  (*m_tick_callback)(              dummy&)=nullptr;
  void  (*m_finish_callback)(            dummy&)=nullptr;

  void*  m_data=nullptr;

  status_value<int>  m_status;

  struct flags{
    static constexpr int              live =   1;
    static constexpr int             sleep =   2;
    static constexpr int             alarm =   4;
    static constexpr int              show =   8;
    static constexpr int             blink =  32;
    static constexpr int         blink_bit =  64;
    static constexpr int    die_when_getup = 128;
    static constexpr int  discard_when_die = 256;
    static constexpr int           discard = 512;
  };


  clock_watch  m_clock_watch;

  uint32_t  m_interval=0;
  uint32_t  m_next_time=0;

  uint32_t  m_last_time=0;
  uint32_t  m_elapsed_time=0;
  uint32_t  m_living_time=0;
  uint32_t  m_getup_count=0;
  uint32_t  m_die_count=0;

  uint32_t  m_blink_show_value=0;
  uint32_t  m_blink_hide_value=0;
  uint32_t  m_blink_counter=0;

public:
  task(void*  ptr=nullptr) noexcept: m_data(ptr){}

  task&  set_name(std::string_view  sv) noexcept{  m_name = sv;  return *this;}
  task&    set_id(std::string_view  sv) noexcept{  m_id   = sv;  return *this;}

  const std::string&  get_name() const noexcept{return m_name;}
  const std::string&    get_id() const noexcept{return m_id;}

  template<typename  T>T*  get_data() const noexcept{return static_cast<T*>(m_data);}

  task&  clear() noexcept;
 
  task&  clear_status() noexcept{  m_status.clear();  return *this;}

  task&  live() noexcept{  m_status.set(flags::live);  return *this;}
  task&   die() noexcept;

  task&  die_when_getup() noexcept{  m_status.set(flags::die_when_getup);  return *this;}

  task&  sleep(           ) noexcept;
  task&  sleep(uint32_t  t) noexcept;
  task&  getup(           ) noexcept;

  task&  show() noexcept{  m_status.set(  flags::show);  return *this;}
  task&  hide() noexcept{  m_status.unset(flags::show);  return *this;}

  task&    blink() noexcept{  m_status.set(  flags::blink);  return *this;}
  task&  unblink() noexcept{  m_status.unset(flags::blink);  return *this;}

  task&  discard() noexcept{  m_status.set(flags::discard);  return *this;}

  task&  discard_when_die() noexcept;

  task&  set_blinking_rate(int  show, int  hide) noexcept;

  bool  is_living()   const noexcept{return m_status.test(flags::live);}
  bool  is_sleeping() const noexcept{return m_status.test(flags::sleep);}
  bool  is_blinking() const noexcept{return m_status.test(flags::blink);}
  bool  is_showing()  const noexcept{return m_status.test(flags::show);}
  bool  is_discarded()  const noexcept{return m_status.test(flags::discard);}

  uint32_t  get_interval(           ) const noexcept{return m_interval                   ;}
  task&     set_interval(uint32_t  t)       noexcept{       m_interval = t;  return *this;}

  uint32_t  get_next_time() const noexcept{return m_next_time;}
  task&  update_next_time() noexcept;

  uint32_t  get_elapsed_time()  const noexcept{return m_elapsed_time;}
  uint32_t  get_living_time()   const noexcept{return m_living_time;}

  uint32_t  get_die_count()   const noexcept{return m_die_count;}
  uint32_t  get_getup_count() const noexcept{return m_getup_count;}

  task&  reset_elapsed_time( uint32_t  t=0) noexcept{  m_elapsed_time  = t;  return *this;}
  task&  reset_living_time(  uint32_t  t=0) noexcept{  m_living_time   = t;  return *this;}

  task&  reset_die_count(  uint32_t  v=0) noexcept{  m_die_count   = v;  return *this;}
  task&  reset_getup_count(uint32_t  v=0) noexcept{  m_getup_count = v;  return *this;}

  task&  set_clock_watch(clock_watch  w) noexcept{  m_clock_watch = w;  return *this;}

  task&  set_data(void*  ptr) noexcept{  m_data = ptr;  return *this;}


  task&  set_draw(void  (*cb)(const canvas&,dummy&)) noexcept{  m_draw_callback = cb;  return *this;}

  template<typename  T>
  task&  set_draw(void  (*cb)(const canvas&,T&)) noexcept
  {
    return set_draw(reinterpret_cast<void(*)(const canvas&,dummy&)>(cb));
  }

  template<typename  T>
  task&  set_draw() noexcept
  {
    return set_draw(T::draw);
  }

  task&  set_tick(void  (*cb)(dummy&)) noexcept{  m_tick_callback = cb;  return *this;}

  template<typename  T>
  task&  set_tick(void  (*cb)(T&)) noexcept
  {
    return set_tick(reinterpret_cast<void(*)(dummy&)>(cb));
  }

  template<typename  T>
  task&  set_tick() noexcept
  {
    return set_tick(T::tick);
  }

  task&  set_finish(void  (*cb)(dummy&)) noexcept{  m_finish_callback = cb;  return *this;}

  template<typename  T>
  task&  set_finish(void  (*cb)(T&)) noexcept
  {
    return set_finish(reinterpret_cast<void(*)(dummy&)>(cb));
  }

  template<typename  T>
  task&  set_finish() noexcept
  {
    return set_finish(T::finish);
  }

  task&  operator()(                 ) noexcept;
  task&  operator()(const canvas&  cv) noexcept;

  task&  finish() noexcept;

};




class execution;
class process;

using execution_callback = void(*)(execution&,dummy&);


struct
execution_entry
{
  uintptr_t  m_nameptr;
  uintptr_t  m_callback;
  uintptr_t  m_data;

  template<typename  T> constexpr uintptr_t  cast(T*  ptr) noexcept{return reinterpret_cast<uintptr_t>(ptr);}

  constexpr execution_entry() noexcept:
  m_nameptr(0), m_callback(0), m_data(0){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&)) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(0){}

  template<typename  T>
  constexpr execution_entry(const char*  name, void(*cb)(execution&,T&), T&  data) noexcept:
  m_nameptr(cast(name)), m_callback(cast(cb)), m_data(cast(&data)){}

  template<typename  T>
  constexpr execution_entry(const char*  name, T&  data) noexcept:
  m_nameptr(cast(name)), m_callback(cast(T::execute)), m_data(cast(&data)){}

  constexpr const char*  get_name() const noexcept{return m_nameptr? reinterpret_cast<const char*>(m_nameptr):"";}

  execution_callback  get_callback()  const noexcept{return reinterpret_cast<execution_callback>(m_callback);}
  dummy*                  get_data()      const noexcept{return reinterpret_cast<dummy*>(m_data);}

  constexpr bool  test_interruption() const noexcept{return get_name()[0] == '*';}

};


class
execution_context
{
  friend class execution;

  uint32_t  m_pc=0;//Program Counter
  uint32_t  m_lc=0;//Level Counter
  uint32_t  m_sp=0;//Stack Pointer
  uint32_t  m_bp=0;//Base Pointer

public:
  uint32_t  get_pc() const noexcept{return m_pc;}
  uint32_t  get_lc() const noexcept{return m_lc;}
  uint32_t  get_sp() const noexcept{return m_sp;}
  uint32_t  get_bp() const noexcept{return m_bp;}

};


class
execution
{
protected:
  std::list<clock*>  m_clock_list;
  std::list<task*>    m_task_list;

  uint32_t  m_pc=0;//Program Counter
  uint32_t  m_lc=0;//Level Counter
  uint32_t  m_sp=0;//Stack Pointer
  uint32_t  m_bp=0;//Base Pointer

  uint32_t  m_last_time;

  color  m_background_color;

  std::vector<uintptr_t>  m_memory;

  std::vector<execution_context>  m_context_stack;

  int  m_jump_value;

  clock  m_equipped_clock;

  status_value<int>  m_status;

  struct flags{
    static constexpr int  verbose    = 1;
    static constexpr int  pc_barrier = 2;
    static constexpr int  jumped     = 4;
  };

  execution() noexcept: m_memory(800){}

  execution(const execution& )=default;
  execution(      execution&&)=default;

  execution&  operator=(const execution& )=default;
  execution&  operator=(      execution&&)=default;

  execution&  clear() noexcept;
  execution&  reset() noexcept;

public:
 ~execution(){clear();}

  operator bool() const noexcept{return m_lc;}

  clock&  get_equipped_clock() noexcept{return m_equipped_clock;}

  execution_context  get_context() const noexcept;

  execution&  push_current_context(      ) noexcept{  m_context_stack.emplace_back(get_context());  return *this;}
  void         pop_current_context(int  v) noexcept;

  int     set_jump(execution_context&  ctx) noexcept;
  void  unset_jump() noexcept{m_status.unset(flags::jumped);}
  void  jump(const execution_context&  ctx, int  v) noexcept;

  bool  is_jumped() const noexcept{return m_status.test(flags::jumped);}

  int  get_jump_value() const noexcept{return m_jump_value;}

  bool  test_verbose_flag() const noexcept{return m_status.test(flags::verbose);}
  execution&    set_verbose_flag() noexcept{  m_status.set(  flags::verbose);  return *this;}
  execution&  unset_verbose_flag() noexcept{  m_status.unset(flags::verbose);  return *this;}

  execution&  add_clock(clock&  cl) noexcept{  m_clock_list.emplace_back(&cl);  return *this;}
  execution&   add_task(task&  tsk) noexcept{  m_task_list.emplace_back(&tsk);  return *this;}

  execution&  remove_clocks_by_name(std::string_view  name) noexcept;
  execution&  remove_tasks_by_name( std::string_view  name) noexcept;

  execution&  remove_clock_by_id(std::string_view  name) noexcept;
  execution&  remove_task_by_id( std::string_view  name) noexcept;

  execution&  remove_dead_tasks() noexcept;

  pointer_wrapper<clock>  get_clock_by_id(std::string_view  name) const noexcept;
  pointer_wrapper< task>   get_task_by_id(std::string_view  name) const noexcept;

  color       get_background_color(        ) const noexcept{return m_background_color                   ;}
  execution&  set_background_color(color  c)       noexcept{       m_background_color = c;  return *this;}

  const uint32_t&  get_pc() const noexcept{return m_pc;}
  const uint32_t&  get_lc() const noexcept{return m_lc;}
  const uint32_t&  get_sp() const noexcept{return m_sp;}
  const uint32_t&  get_bp() const noexcept{return m_bp;}

  execution&  push_frame(std::initializer_list<execution_entry>  ls, const char*  name=nullptr) noexcept;
  execution&   pop_frame() noexcept;

  execution&  operator++() noexcept;

  void  print() const noexcept;

};


class
process: public execution
{
  std::string  m_name;

  uint32_t  m_interval=0;
  uint32_t  m_next_time=0;

  canvas  m_canvas;

  void  update_clocks() noexcept;
  void  process_tasks(const canvas*  cv) noexcept;

public:
  process() noexcept{}

  bool  is_busy() const noexcept{return *this;}

  std::string_view  get_name() const noexcept{return m_name.size()? m_name:"NoName";}

  process&  assign(std::string_view  name, execution_entry  start) noexcept;

  process&  set_canvas(const canvas&  cv) noexcept{  m_canvas = cv;  return *this;}
  process&  set_interval(uint32_t  t) noexcept{  m_interval = t;  return *this;}

  bool  operator()() noexcept;

};




}




#endif




