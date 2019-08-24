#ifndef SNS_RECORD_HPP_WAS_INCLUDED
#define SNS_RECORD_HPP_WAS_INCLUDED


#include<cstdint>
#include<cstdio>
#include<cstring>
#include<cctype>
#include<vector>
#include<memory>
#include<string>
#include<string_view>


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

  constexpr bool  operator< (timestamp  rhs) const noexcept{return m_value <  rhs.m_value;}
  constexpr bool  operator<=(timestamp  rhs) const noexcept{return m_value <= rhs.m_value;}
  constexpr bool  operator> (timestamp  rhs) const noexcept{return m_value >  rhs.m_value;}
  constexpr bool  operator>=(timestamp  rhs) const noexcept{return m_value >= rhs.m_value;}
  constexpr bool  operator==(timestamp  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(timestamp  rhs) const noexcept{return m_value != rhs.m_value;}

  constexpr uint64_t  get_value() const noexcept{return m_value;}

};


class
record
{
  uint64_t  m_index=0;

  timestamp  m_timestamp;

  std::string  m_content;

public:
  record() noexcept{}
  record(uint64_t  i, timestamp  ts, std::string_view  sv) noexcept: m_index(i), m_timestamp(ts), m_content(sv){}

  bool  operator< (timestamp  ts) const noexcept{return m_timestamp <  ts;}
  bool  operator<=(timestamp  ts) const noexcept{return m_timestamp <= ts;}
  bool  operator> (timestamp  ts) const noexcept{return m_timestamp >  ts;}
  bool  operator>=(timestamp  ts) const noexcept{return m_timestamp >= ts;}
  bool  operator==(timestamp  ts) const noexcept{return m_timestamp == ts;}
  bool  operator!=(timestamp  ts) const noexcept{return m_timestamp != ts;}

  uint64_t  get_index() const noexcept{return m_index;}

  timestamp  get_timestamp() const noexcept{return m_timestamp;}

  const std::string&   get_content() const noexcept{return m_content;}

  void  print() const noexcept;

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

  void  print() const noexcept
  {
    printf("%4d年%2d月%2d日:%2d時%2d分%2d秒",
      1+get_year(),
      1+get_month(),
      1+get_day(),
      get_hour(),
      get_minute(),
      get_second()
    );
  }

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


template<typename  T> class  table;
template<typename  T> class  index_list;


template<typename  T>
class
table
{
  timestamp  m_last_timestamp;

  std::vector<std::unique_ptr<T>>  m_container;

  range  search_internal(const std::unique_ptr<T>*  base_pointer, uint64_t  bottom, uint64_t  top, timestamp  ts) const noexcept
  {
    auto  length = 1+(top-bottom);

      if(length < 4)
      {
        return range(bottom,top);
      }


    uint64_t  center = bottom+(length/2);

    auto  sample_ts = base_pointer[center]->get_timestamp();

    return (sample_ts < ts)? search_internal(base_pointer,bottom+1,top  ,ts)
          :(sample_ts > ts)? search_internal(base_pointer,bottom  ,top-1,ts)
          :                  range(bottom,top);
  }


  range  search(timestamp  ts) const noexcept
  {
    int  n = m_container.size();

    return (n == 0)? ranges::wrong
          :(n == 1)? range(0,0)
          :(n == 2)? range(0,1)
          :search_internal(m_container.data(),0,n-1,ts);
  }


  bool  fix_bottom(uint64_t&  bottom, uint64_t  top, timestamp  a, timestamp  b) const noexcept
  {
      for(;;)
      {
        auto&  t = *m_container[bottom];

          if((t >= a) &&
             (t <= b))
          {
            return true;
          }


          if(++bottom > top)
          {
            return false;
          }
      }
  }


  bool  fix_top(uint64_t  bottom, uint64_t&  top, timestamp  a, timestamp  b) const noexcept
  {
      for(;;)
      {
        auto&  t = *m_container[top];

          if((t >= a) &&
             (t <= b))
          {
            return true;
          }


          if(--top < bottom)
          {
            return false;
          }
      }
  }

public:
  int  get_number_of_records() const noexcept{return m_container.size();}

  const std::unique_ptr<T>&  operator[](uint64_t  i) const noexcept{return m_container[i];}

  const std::unique_ptr<T>&  append(timestamp  ts, std::string_view  sv) noexcept
  {
      if(ts <= m_last_timestamp)
      {
        static const std::unique_ptr<T>  null;

        printf("table: timestamp error\n");

        return null;
      }


    m_container.emplace_back(std::make_unique<T>(1+m_container.size(),ts,sv));

    m_last_timestamp = ts;

    return m_container.back();
  }


  index_list<T>  make_index_list() const noexcept
  {
    return index_list<T>(*this);
  }

  index_list<T>  make_index_list(timestamp  a, timestamp  b) const noexcept
  {
      if(a > b)
      {
        std::swap(a,b);
      }


      if(m_container.size())
      {
        auto  a_result = search(a);
        auto  b_result = search(b);

        auto&  bottom = a_result.bottom;
        auto&     top = b_result.top;

          if(fix_bottom(bottom,top,a,b) && 
             fix_top(   bottom,top,a,b))
          {
            return index_list<T>(*this,range(bottom,top));
          }
      }


    return index_list<T>(*this);
  }

  const std::unique_ptr<T>*  begin() const noexcept{return m_container.data();}
  const std::unique_ptr<T>*    end() const noexcept{return m_container.data()+m_container.size();}

};


template<typename  T>
class
index_list
{
  const table<T>&  m_table;

  std::vector<uint64_t>  m_container;

  void  copy_indexes() noexcept
  {
    m_container.clear();

    auto  n = m_table.get_number_of_records();

      for(auto  i = 0;   i < n;  ++i)
      {
        m_container.emplace_back(i);
      }
  }

  void  copy_indexes(range   r) noexcept
  {
    m_container.clear();

      if(r)
      {
          for(auto  i = r.bottom;   i <= r.top;  ++i)
          {
            m_container.emplace_back(i);
          }
      }
  }

public:
  index_list(const table<T>&  tbl          ) noexcept: m_table(tbl){copy_indexes( );}
  index_list(const table<T>&  tbl, range  r) noexcept: m_table(tbl){copy_indexes(r);}

  const uint64_t&  operator[](int  i) const noexcept{return m_container[i];}

  int  get_number_of_indexes() const noexcept{return m_container.size();}

  const uint64_t*  begin() const noexcept{return m_container.data();}
  const uint64_t*    end() const noexcept{return m_container.data()+m_container.size();}

  template<typename  Tester>
  index_list&  filter(Tester  t) noexcept
  {
    std::vector<uint64_t>  tmp;

      for(auto  i: m_container)
      {
          if(t(m_table[i]->get_content().data()))
          {
            tmp.emplace_back(i);
          }
      }


    m_container = std::move(tmp);

    return *this;
  }

};


class
tester
{
  const char*  m_condition;

public:
  constexpr tester(const char*  cond) noexcept: m_condition(cond){}

  bool  operator()(const char*  content) const noexcept;

};


}




#endif




