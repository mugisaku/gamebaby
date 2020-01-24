#ifndef SNS_HPP_WAS_INCLUDED
#define SNS_HPP_WAS_INCLUDED


#include<cstdint>
#include<cinttypes>
#include<cstdio>
#include<string>
#include<string_view>
#include<list>
#include<vector>
#include<memory>
#include"libgbstd/sha256.hpp"
#include"libgbstd/misc.hpp"
#include"libgbstd/utility.hpp"




namespace gbstd{
namespace sns{




namespace times{
constexpr uint64_t  g_second_value = 1;
constexpr uint64_t  g_minute_value = g_second_value*60;
constexpr uint64_t  g_hour_value   = g_minute_value*60;
constexpr uint64_t  g_day_value    = g_hour_value*24;
constexpr uint64_t  g_month_value  = g_day_value*4;
constexpr uint64_t  g_year_value   = g_month_value*12;

constexpr uint64_t    year(int  v) noexcept{return   g_year_value*(v? v-1:1);}
constexpr uint64_t   month(int  v) noexcept{return  g_month_value*(v? v-1:1);}
constexpr uint64_t     day(int  v) noexcept{return    g_day_value*(v? v-1:1);}
constexpr uint64_t    hour(int  v) noexcept{return   g_hour_value*v;}
constexpr uint64_t  minute(int  v) noexcept{return g_minute_value*v;}
constexpr uint64_t  second(int  v) noexcept{return g_second_value*v;}


}


class
timestamp
{
  uint64_t  m_value;

public:
  constexpr timestamp(uint64_t  v=0) noexcept: m_value(v){}

  constexpr operator bool() const noexcept{return m_value;}

  constexpr bool  operator< (timestamp  rhs) const noexcept{return m_value <  rhs.m_value;}
  constexpr bool  operator<=(timestamp  rhs) const noexcept{return m_value <= rhs.m_value;}
  constexpr bool  operator> (timestamp  rhs) const noexcept{return m_value >  rhs.m_value;}
  constexpr bool  operator>=(timestamp  rhs) const noexcept{return m_value >= rhs.m_value;}
  constexpr bool  operator==(timestamp  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(timestamp  rhs) const noexcept{return m_value != rhs.m_value;}

  timestamp&  operator+=(uint64_t  v) noexcept
  {
    m_value += v;

    return *this;
  }

  constexpr uint64_t  get_value() const noexcept{return m_value;}

};


class
time_object
{
  int    m_year=0;
  int   m_month=0;
  int     m_day=0;

  int       m_hour=0;
  int     m_minute=0;
  int     m_second=0;

  static uint64_t  div(uint64_t&  n, int  amount) noexcept
  {
    auto  v = n/amount;

    n %= amount;

    return v;
  }

public:
  time_object() noexcept{}
  time_object(timestamp  ts) noexcept{assign(ts);}

  time_object&  operator=(timestamp  ts) noexcept{return assign(ts);}

  time_object&  assign(timestamp  ts) noexcept
  {
    auto   n = ts.get_value();

    m_year   = div(n,  times::g_year_value);
    m_month  = div(n, times::g_month_value);
    m_day    = div(n,   times::g_day_value);
    m_hour   = div(n,  times::g_hour_value);
    m_minute = div(n,times::g_minute_value);
    m_second = div(n,times::g_second_value);

    return *this;
  }

  timestamp  make_timestamp() const noexcept
  {
    return timestamp( (static_cast<uint64_t>(m_year  )*times::g_year_value  )
                     +(static_cast<uint64_t>(m_month )*times::g_month_value )
                     +(static_cast<uint64_t>(m_day   )*times::g_day_value   )
                     +(static_cast<uint64_t>(m_hour  )*times::g_hour_value  )
                     +(static_cast<uint64_t>(m_minute)*times::g_minute_value)
                     +(static_cast<uint64_t>(m_second)*times::g_second_value));
  }

  time_object&  clear() noexcept{  std::memset(this,0,sizeof(*this));  return *this;}

  time_object&  set_year(int  v)   noexcept{  m_year   = v? v-1:0;  return *this;}
  time_object&  set_month(int  v)  noexcept{  m_month  = v? v-1:0;  return *this;}
  time_object&  set_day(int  v)    noexcept{  m_day    = v? v-1:0;  return *this;}
  time_object&  set_hour(int  v)   noexcept{  m_hour   = v;  return *this;}
  time_object&  set_minute(int  v) noexcept{  m_minute = v;  return *this;}
  time_object&  set_second(int  v) noexcept{  m_second = v;  return *this;}

  int  get_year()   const noexcept{return m_year  ;}
  int  get_month()  const noexcept{return m_month ;}
  int  get_day()    const noexcept{return m_day   ;}
  int  get_hour()   const noexcept{return m_hour  ;}
  int  get_minute() const noexcept{return m_minute;}
  int  get_second() const noexcept{return m_second;}

};




inline
std::string
to_string(const time_object&  to) noexcept
{
  char  buf[256];

  snprintf(buf,sizeof(buf),"%4d/%2d/%2d.%02d:%02d:%02d",
      1+to.get_year(),
      1+to.get_month(),
      1+to.get_day(),
      to.get_hour(),
      to.get_minute(),
      to.get_second()
    );


  return std::string(buf);
}


inline
std::string
to_string(timestamp  ts) noexcept
{
  time_object  to(ts);

  return to_string(to);
}




class
record
{
  uint64_t  m_index=0;

  timestamp  m_timestamp;

  std::string  m_content;

  sha256_hash  m_seed_hash;

public:
  record() noexcept{}
  record(uint64_t  i, timestamp  ts, std::string_view  sv, const sha256_hash&  seed_hash) noexcept:
  m_index(i),
  m_timestamp(ts),
  m_content(sv),
  m_seed_hash(seed_hash){}

  bool  operator< (timestamp  ts) const noexcept{return m_timestamp <  ts;}
  bool  operator<=(timestamp  ts) const noexcept{return m_timestamp <= ts;}
  bool  operator> (timestamp  ts) const noexcept{return m_timestamp >  ts;}
  bool  operator>=(timestamp  ts) const noexcept{return m_timestamp >= ts;}
  bool  operator==(timestamp  ts) const noexcept{return m_timestamp == ts;}
  bool  operator!=(timestamp  ts) const noexcept{return m_timestamp != ts;}

  uint64_t  get_index() const noexcept{return m_index;}

  timestamp  get_timestamp() const noexcept{return m_timestamp;}

  const sha256_hash&  get_seed_hash() const noexcept{return m_seed_hash;}

  sha256_hash  generate_hash() const noexcept;

  const std::string&   get_content() const noexcept{return m_content;}

  void  print() const noexcept;

};




struct
range
{
  uint64_t  bottom=0;
  uint64_t     top=0;

  constexpr range(uint64_t  b=0, uint64_t  t=0) noexcept: bottom(b), top(t){}

  constexpr operator bool() const noexcept{return bottom <= top;}

  constexpr uint64_t  get_length() const noexcept{return 1+(top-bottom);}

};


namespace ranges{
constexpr range  wrong(1,0);
}


class record_table;

class
index_list
{
  std::vector<uint64_t>  m_container;

public:
  index_list(range  r) noexcept{assign(r);}

  index_list&  operator=(range  r) noexcept{return assign(r);}
  index_list&  assign(range  r) noexcept;

  const uint64_t&  operator[](int  i) const noexcept{return m_container[i];}

  int  get_number_of_indexes() const noexcept{return m_container.size();}

  const uint64_t*  begin() const noexcept{return m_container.data();}
  const uint64_t*    end() const noexcept{return m_container.data()+m_container.size();}

  template<typename  Tester>
  index_list&
  filter(const record_table&  tbl, Tester  t) noexcept;


};




class
record_table
{
  timestamp  m_last_timestamp;

  std::vector<record*>  m_container;

  range  search_internal(const record* const*  base_pointer, uint64_t  bottom, uint64_t  top, timestamp  ts) const noexcept;

  bool  fix_bottom(uint64_t&  bottom, uint64_t   top, timestamp  a, timestamp  b) const noexcept;
  bool  fix_top(   uint64_t   bottom, uint64_t&  top, timestamp  a, timestamp  b) const noexcept;

public:
 ~record_table(){clear();}

  void  clear() noexcept;

  int  get_number_of_records() const noexcept{return m_container.size();}

  operator bool() const noexcept{return m_container.size();}

  const record&  operator[](uint64_t  i) const noexcept{return *m_container[i];}

  const record&  get_first() const noexcept{return *m_container.front();}
  const record&  get_last()  const noexcept{return *m_container.back();}

  void  append(timestamp  ts, std::string_view  sv) noexcept;

  range  search(timestamp  ts) const noexcept;

  range  make_range(timestamp  a, timestamp  b) const noexcept;

  index_list  make_index_list(timestamp  a, timestamp  b) const noexcept
  {
    return index_list(make_range(a,b));
  }

  class iterator{
    const record* const*  m_pp;
  public:
    constexpr iterator(const record* const*   pp) noexcept: m_pp(pp){}

    constexpr const record&  operator*() const noexcept{return **m_pp;}
    constexpr bool           operator!=(iterator  rhs) const noexcept{return m_pp != rhs.m_pp;}

    iterator&  operator++() noexcept{  ++m_pp;  return *this;}

  };

  iterator  begin() const noexcept{return iterator(m_container.data()                   );}
  iterator    end() const noexcept{return iterator(m_container.data()+m_container.size());}

};


class
tester
{
  const char*  m_condition;

public:
  constexpr tester(const char*  cond) noexcept: m_condition(cond){}

  bool  operator()(const char*  content) const noexcept;

};



template<typename  Tester>
index_list&
index_list::
filter(const record_table&  tbl, Tester  t) noexcept
{
  std::vector<uint64_t>  tmp;

    for(auto  i: m_container)
    {
        if(t(tbl[i].get_content().data()))
        {
          tmp.emplace_back(i);
        }
    }


  m_container = std::move(tmp);

  return *this;
}




class
sex
{
  int  m_value;

public:
  constexpr sex(int  v=0) noexcept: m_value(v){}

  constexpr bool  is_unspecified() const noexcept{return m_value == 0;}
  constexpr bool  is_male()        const noexcept{return m_value == 1;}
  constexpr bool  is_female()      const noexcept{return m_value == 2;}

  constexpr std::string_view  get_name() const noexcept
  {
    return (m_value == 0)? "unspecified"
          :(m_value == 1)? "male"
          :(m_value == 2)? "female"
          :"invalid";
  }

};


namespace sexes{
constexpr sex  unspecified = sex(0);
constexpr sex         male = sex(1);
constexpr sex       female = sex(2);
}


class platform;
class  account;


class
account_observer
{
  struct private_data;

  private_data*  m_data=nullptr;

public:
  account_observer() noexcept{}
  account_observer(account&  ref) noexcept;
  account_observer(const account_observer&   rhs) noexcept{assign(rhs);}
  account_observer(      account_observer&&  rhs) noexcept{assign(std::move(rhs));}
 ~account_observer(){unrefer();}

  operator bool() const noexcept;

  account_observer&  operator=(const account_observer&   rhs) noexcept{return assign(rhs);}
  account_observer&  operator=(      account_observer&&  rhs) noexcept{return assign(std::move(rhs));}

  account&  operator *() const noexcept;
  account*  operator->() const noexcept;

  bool  operator==(const account_observer&  rhs) const noexcept{return m_data == rhs.m_data;}
  bool  operator!=(const account_observer&  rhs) const noexcept{return m_data != rhs.m_data;}

  void  die() noexcept;

  void  unrefer() noexcept;

  account_observer&  assign(const account_observer&   rhs) noexcept;
  account_observer&  assign(      account_observer&&  rhs) noexcept;

};


class
account
{
  account_observer  m_observer;

  uint64_t  m_index;

  std::string  m_name;

  uint64_t  m_birth_date;

  sex  m_sex;

  timestamp  m_create_timestamp;
  timestamp  m_modify_timestamp;

  record_table  m_record_table;

  std::list<account_observer>    m_follow_list;
  std::list<account_observer>  m_follower_list;
  std::list<account_observer>     m_block_list;
  std::list<account_observer>      m_mute_list;

public:
  account(uint64_t  i, timestamp  ts) noexcept: m_observer(*this), m_index(i), m_create_timestamp(ts){}
  account(const account& ) noexcept=delete;
  account(      account&&) noexcept=delete;
 ~account(){clear();}

  account&  operator=(const account& ) noexcept=delete;
  account&  operator=(      account&&) noexcept=delete;

  void  clear() noexcept;

  uint64_t  get_index() const noexcept{return m_index;}

  const std::string&  get_name(                    ) const noexcept{return m_name                    ;}
  account&            set_name(std::string_view  sv)       noexcept{       m_name = sv;  return *this;}

  sex        get_sex(      ) const noexcept{return m_sex                   ;}
  account&   set_sex(sex  s)       noexcept{       m_sex = s;  return *this;}

  const account_observer&  get_observer() const noexcept{return m_observer;}

  const record_table&  get_record_table() const noexcept{return m_record_table;}

  account&  add_record(timestamp  ts, std::string_view  sv) noexcept;

  timestamp  get_last_post_timestamp() const noexcept;

  void  add_follow(  const account_observer&  obs) noexcept;
  void  add_follower(const account_observer&  obs) noexcept;
  void  add_block(   const account_observer&  obs) noexcept;
  void  add_mute(    const account_observer&  obs) noexcept;

  bool  test_follow(  const account_observer&  obs) const noexcept;
  bool  test_follower(const account_observer&  obs) const noexcept;
  bool   test_block(  const account_observer&  obs) const noexcept;
  bool    test_mute(  const account_observer&  obs) const noexcept;

  void  remove_follow(  const account_observer&  obs) noexcept;
  void  remove_follower(const account_observer&  obs) noexcept;
  void  remove_block(   const account_observer&  obs) noexcept;
  void  remove_mute(    const account_observer&  obs) noexcept;

  void  print() const noexcept;

};




/*
class
article::
link
{
  uint64_t  m_account_index;
  uint64_t    m_entry_index;

public:
  constexpr link(uint64_t  acc_i=0, uint64_t  ent_i=0) noexcept:
  m_account_index(acc_i),
  m_entry_index(ent_i){}

  constexpr uint64_t  get_account_index() const noexcept{return m_account_index;}
  constexpr uint64_t  get_entry_index()   const noexcept{return m_entry_index;}

};
*/




class
platform
{
  std::vector<std::unique_ptr<account>>  m_account_table;

public:
  const account_observer&  create_account(timestamp  ts) noexcept;

  const account_observer&  get_account_by_index(uint64_t  i) const noexcept{return m_account_table[i]->get_observer();}

  void  print() const noexcept;

};


class
article
{
  friend class  timeline     ;
  friend class  timeline_view;

  account_observer  m_observer;

  timestamp  m_timestamp;

  std::u16string  m_name;
  std::u16string  m_date;

  std::string  m_content;

  article*   m_forward=nullptr;
  article*  m_backward=nullptr;

  article() noexcept{}
  article(const account_observer&  obs, const record&  rec) noexcept{assign(obs,rec);}

  article&  assign(const account_observer&  obs, const record&  rec) noexcept;

public:
  const account_observer&  get_observer() const noexcept{return m_observer;}

  timestamp  get_timestamp() const noexcept{return m_timestamp;}

  const std::u16string&  get_name() const noexcept{return m_name;}
  const std::u16string&  get_date() const noexcept{return m_date;}
  const std::string&  get_content() const noexcept{return m_content;}

  static void  link(article*  f, article*  b) noexcept;

};


class
timeline_node
{
  account_observer  m_observer;

  struct flags{
    static constexpr int    initialize = 1;
    static constexpr int          lock = 2;
    static constexpr int  reach_bottom = 4;

  };

  status_value<int>  m_status;

  uint64_t     m_head_index;
  uint64_t  m_pretail_index;

  bool  initialize() noexcept;

public:
  timeline_node() noexcept{}
  timeline_node(const account_observer&  obs) noexcept{set_observer(obs);}

  operator bool() const noexcept{return m_observer->get_record_table();}

  timeline_node&           set_observer(const account_observer&  obs) noexcept;
  const account_observer&  get_observer() const noexcept{return m_observer;}

  timeline_node&  reset() noexcept;

  timeline_node&    lock() noexcept{  m_status.set(flags::lock);  return *this;}
  timeline_node&  unlock() noexcept{  m_status.unset(flags::lock);  return *this;}

  bool  is_locked() const noexcept{return m_status.test(flags::lock);}

  void  advance_head_index() noexcept{++m_head_index;}
  void  advance_pretail_index() noexcept;

  const record*  get_next_head() noexcept;
  const record*  get_next_tail() noexcept;

};


class
timeline
{
  std::vector<timeline_node>  m_nodes;

  std::vector<article*>  m_temporary_table;

  article*  m_top=nullptr;
  article*  m_bottom=nullptr;

  int  m_number_of_articles=0;

  timestamp  m_head_ts;
  timestamp  m_tail_ts;

  void  push_forward(article*  a) noexcept;
  void  push_backward(article*  a) noexcept;

public:
   timeline() noexcept{}
   timeline(const timeline&   rhs) noexcept=delete;//{assign(rhs);}
   timeline(      timeline&&  rhs) noexcept{assign(std::move(rhs));}
  ~timeline(){clear();}

  timeline&  operator=(const timeline&   rhs) noexcept=delete;//{return assign(rhs);}
  timeline&  operator=(      timeline&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept{return m_number_of_articles;}


//  timeline&  assign(const timeline&   rhs) noexcept;
  timeline&  assign(      timeline&&  rhs) noexcept;

  void  clear() noexcept;

  void  add(const account_observer&  obs) noexcept{m_nodes.emplace_back(obs);}

  int  get_number_of_articles() const noexcept{return m_number_of_articles;}

  void  ready(timestamp  ts) noexcept;

  uint64_t  read_forward()  noexcept;
  uint64_t  read_backward() noexcept;

  class iterator{
  protected:  const article*  m_pointer;
  public:
    constexpr iterator(const article*  ptr=nullptr) noexcept: m_pointer(ptr){}

    constexpr operator bool() const noexcept{return m_pointer;}

    constexpr const article&  operator*()  const noexcept{return *m_pointer;}
    constexpr const article*  operator->() const noexcept{return  m_pointer;}

    constexpr bool  operator==(const iterator&  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
    constexpr bool  operator!=(const iterator&  rhs) const noexcept{return m_pointer != rhs.m_pointer;}

    iterator&  operator+=(int  n) noexcept{  *this = (*this)+n;  return *this;}
    iterator&  operator-=(int  n) noexcept{  *this = (*this)-n;  return *this;}

    iterator&  operator++() noexcept{  m_pointer = m_pointer->m_forward ;   return *this;}
    iterator&  operator--() noexcept{  m_pointer = m_pointer->m_backward;  return *this;}

    iterator  operator++(int) noexcept{  auto  p = m_pointer;  m_pointer = p->m_forward;   return p;}
    iterator  operator--(int) noexcept{  auto  p = m_pointer;  m_pointer = p->m_backward;  return p;}

    iterator  operator+(int  n) const noexcept;
    iterator  operator-(int  n) const noexcept;

    const article*  to_pointer() const noexcept{return m_pointer;}

  };

  iterator  begin() const noexcept{return m_bottom;}
  iterator    end() const noexcept{return nullptr;}

  class reverse_iterator: public iterator{
  public:
    using iterator::iterator;

    constexpr reverse_iterator(iterator  it) noexcept: iterator(&*it){}

    reverse_iterator&  operator+=(int  n) noexcept{  *this = (*this)+n;  return *this;}
    reverse_iterator&  operator-=(int  n) noexcept{  *this = (*this)-n;  return *this;}

    reverse_iterator&  operator++() noexcept{  m_pointer = m_pointer->m_backward;  return *this;}
    reverse_iterator&  operator--() noexcept{  m_pointer = m_pointer->m_forward;   return *this;}

    reverse_iterator  operator++(int) noexcept{  auto  p = m_pointer;  m_pointer = p->m_backward;  return p;}
    reverse_iterator  operator--(int) noexcept{  auto  p = m_pointer;  m_pointer = p->m_forward;   return p;}

    reverse_iterator  operator+(int  n) const noexcept;
    reverse_iterator  operator-(int  n) const noexcept;

  };

  reverse_iterator  rbegin() const noexcept{return m_top;}
  reverse_iterator    rend() const noexcept{return nullptr;}

};


class
timeline_view
{
  const timeline*  m_timeline=nullptr;

  const article*  m_head;
  const article*  m_tail;

  int  m_distance=0;

public:
  timeline_view() noexcept;
  timeline_view(const timeline&  tl) noexcept{assign(tl);}

  timeline_view&  operator=(const timeline&  tl) noexcept{return assign(tl);}
  timeline_view&  assign(const timeline&  tl) noexcept;

  timeline_view&  reset() noexcept;

  int  get_number_of_articles() const noexcept;

  int  set_distance(int  n) noexcept;
  int  get_distance(      ) noexcept{return m_distance;}

  void  move(int  n=1) noexcept;

  timeline::iterator  begin() const noexcept{return m_tail;}
  timeline::iterator    end() const noexcept{return m_head? m_head->m_forward:nullptr;}

  timeline::reverse_iterator  rbegin() const noexcept{return m_head;}
  timeline::reverse_iterator    rend() const noexcept{return m_tail? m_tail->m_backward:nullptr;}

};




}}




#endif




