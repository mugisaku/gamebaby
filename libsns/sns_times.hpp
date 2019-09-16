#ifndef SNS_TIMES_HPP_WAS_INCLUDED
#define SNS_TIMES_HPP_WAS_INCLUDED


#include<cstdint>
#include<cstdio>
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




}




#endif




