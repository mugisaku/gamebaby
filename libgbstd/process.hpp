#ifndef gbstd_process_HPP
#define gbstd_process_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/misc.hpp"
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
task
{
  std::string  m_name;
  std::string  m_id;

  status_value<int>  m_status;

  struct flags{
    static constexpr int              live =   1;
    static constexpr int             sleep =   2;
    static constexpr int             alarm =   4;
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

  void  redraw(const canvas&  cv) noexcept;

public:
 virtual ~task(){}

  task&  set_name(std::string_view  sv) noexcept{  m_name = sv;  return *this;}
  task&    set_id(std::string_view  sv) noexcept{  m_id   = sv;  return *this;}

  const std::string&  get_name() const noexcept{return m_name;}
  const std::string&    get_id() const noexcept{return m_id;}

  task&  reset() noexcept;
 
  task&  clear_status() noexcept{  m_status.clear();  return *this;}

  task&  live() noexcept{  m_status.set(flags::live);  return *this;}
  task&   die() noexcept;

  task&  die_when_getup() noexcept{  m_status.set(flags::die_when_getup);  return *this;}

  task&  sleep(           ) noexcept;
  task&  sleep(uint32_t  t) noexcept;
  task&  getup(           ) noexcept;

  task&  discard() noexcept{  m_status.set(flags::discard);  return *this;}

  task&  discard_when_die() noexcept;

  bool  is_living()   const noexcept{return m_status.test(flags::live);}
  bool  is_sleeping() const noexcept{return m_status.test(flags::sleep);}
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

  void  operator()(const canvas*  cv) noexcept;

  virtual void    work(                 ) noexcept{}
  virtual void  render(const canvas&  cv) noexcept{}
  virtual void  finish() noexcept{}

};




template<typename  T>
class
basic_manager
{
public:
  using list_type = std::list<T*>;

private:
  list_type&  m_list;

public:
  basic_manager(list_type&  ls) noexcept: m_list(ls){}

  int  get_number_of_elements() const noexcept{return m_list.size();}

  basic_manager&  add(T&  e) noexcept{  m_list.emplace_back(&e);  return *this;}
  basic_manager&  add(std::initializer_list<std::reference_wrapper<T>>  ls) noexcept
  {
      for(auto&  ref: ls)
      {
        add(ref.get());
      }


    return *this;
  }

  pointer_wrapper<T>  get_element_by_id(std::string_view  sv) const noexcept
  {
      for(auto  ptr: m_list)
      {
          if(ptr->get_id() == sv)
          {
            return ptr;
          }
      }


    return nullptr;
  }

  std::vector<pointer_wrapper<T>>  get_elements_by_name(std::string_view  sv) const noexcept
  {
    std::vector<pointer_wrapper<T>>  buf;

      for(auto  ptr: m_list)
      {
          if(ptr->get_name() == sv)
          {
            buf.emplace_back(ptr);
          }
      }


    return std::move(buf);
  }

};


class process;

template<typename  T>using procedure = void(T::*)(process&);
	
class
section: public task
{
  friend class process;

  basic_manager<timer>::list_type  m_timer_list;
  basic_manager<clock>::list_type  m_clock_list;
  basic_manager<task>::list_type    m_task_list;

  procedure<section>  m_procedure=nullptr;

  int  m_return_value=0;

  virtual void  startup(process&  proc) noexcept{}
  virtual void  cleanup(process&  proc) noexcept{}

  void  operator()(process&  proc) noexcept{if(m_procedure){(this->*m_procedure)(proc);}}

public:
  operator bool() const noexcept{return m_procedure;}

  int  get_return_value() const noexcept{return m_return_value;}

  basic_manager<timer>  get_timer_manager() noexcept{return basic_manager<timer>(m_timer_list);}
  basic_manager<clock>  get_clock_manager() noexcept{return basic_manager<clock>(m_clock_list);}
  basic_manager<task>    get_task_manager() noexcept{return basic_manager<task>(  m_task_list);}

  template<typename  T>section&  change(procedure<T>  procd) noexcept
  {  m_procedure = static_cast<procedure<section>>(procd);  return *this;}

  section&  change(procedure<section>  procd) noexcept{  m_procedure = procd;  return *this;}

};


class
process
{
  basic_manager<timer>::list_type  m_timer_list;
  basic_manager<clock>::list_type  m_clock_list;
  basic_manager<task>::list_type    m_task_list;

  std::vector<section*>  m_section_stack;

  section*  m_current_section=nullptr;

  uint32_t  m_interval=0;
  uint32_t  m_next_time=0;
  uint32_t  m_last_time=0;

  color  m_background_color=colors::black;

  canvas  m_canvas;

  struct flags{
    static constexpr int       busy = 1;
    static constexpr int  interrupt = 2;
  };

  status_value<int>  m_status;

  void  update_clocks(uint32_t  diff) noexcept;
  void  process_timers(uint32_t  diff) noexcept;
  void  process_tasks(const canvas*  cv) noexcept;

public:
  operator bool() const noexcept{return m_current_section;}

  basic_manager<timer>  get_timer_manager() noexcept{return basic_manager<timer>(m_timer_list);}
  basic_manager<clock>  get_clock_manager() noexcept{return basic_manager<clock>(m_clock_list);}
  basic_manager<task>    get_task_manager() noexcept{return basic_manager<task>(  m_task_list);}

  void  interrupt() noexcept{m_status.set(flags::interrupt);}

  process&  push(section&  sec) noexcept;
  process&   pop(int   value=0) noexcept;

  process&  set_canvas(const canvas&  cv) noexcept{  m_canvas = cv;  return *this;}
  process&  set_interval(uint32_t  t) noexcept{  m_interval = t;  return *this;}

  bool  operator()() noexcept;

};




}




#endif




