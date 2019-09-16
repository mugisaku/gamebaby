#ifndef utility_hpp_was_included
#define utility_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstdint>
#include<random>
#include<stdexcept>


#ifndef report
#define report printf("[report %8d] %s, %s, %d\n",gbstd::reporting_counter++,__FILE__,__func__,__LINE__);
#endif




namespace gbstd{


extern int  reporting_counter;


inline void  printnl() noexcept{printf("\n");}


constexpr double  g_pi = 3.14159265358979323846;

constexpr int  g_sin_table_length = 1024;
constexpr int  g_cos_table_length = 1024;

extern const double  g_sin_table[g_sin_table_length];
extern const double  g_cos_table[g_cos_table_length];


class dummy{};


template<typename  T>
inline void
destruct(T&  t) noexcept
{
  t.~T();
}


template<typename  T>
inline T*
duplicate(const T*  src) noexcept
{
  return src? new T(*src):nullptr;
}


template<typename  T>
inline T*
duplicate(const T*  src, size_t  n) noexcept
{
    if(src)
    {
      auto  p = new T[n];

      auto  dst = p;

        while(n--)
        {
          *dst++ = *src++;
        }


      return p;
    }


  return nullptr;
}


bool   is_png(const void*  ptr) noexcept;
bool  is_webp(const void*  ptr) noexcept;


void  printf_with_indent(int  indent, const char*  fmt, ...) noexcept;


class
validity
{
  bool  m_value=true;

public:
  operator bool() const noexcept{return m_value;}

  void   enable() noexcept{m_value =  true;}
  void  disable() noexcept{m_value = false;}

};




#ifdef EMSCRIPTEN
void  show_github_link() noexcept;
void  show_twitter_link() noexcept;
void  set_caption(const char*  s) noexcept;
void  set_description(const char*  s) noexcept;

void    update_common_blob(const uint8_t*  ptr, size_t  size) noexcept;
void  download_common_blob(const char*  filename) noexcept;

int  get_number_of_dropped_files() noexcept;
std::vector<uint8_t>  pop_front_dropped_file() noexcept;
#endif




extern std::random_device
random_device;


extern std::default_random_engine
random_number_engine;


class
normal_rand
{
  std::normal_distribution<>  m_dist;
//mean平均
//stddev標準偏差(+-)

public:
  normal_rand() noexcept{}
  normal_rand(double  mean, double  stddev) noexcept{reset(mean,stddev);}

  void  reset(double  mean, double  stddev) noexcept{m_dist = std::normal_distribution<>(mean,stddev);}

  double  operator()() noexcept{return m_dist(random_number_engine);}

};


class
uniform_rand
{
  std::uniform_int_distribution<>  m_dist;

public:
  uniform_rand() noexcept{}
  uniform_rand(int  min, int  max) noexcept{reset(min,max);}

  void  reset(int  min, int  max) noexcept{m_dist = std::uniform_int_distribution<>(min,max);}

  int  operator()() noexcept{return m_dist(random_number_engine);}

};


class
uniform_real_rand
{
  std::uniform_real_distribution<>  m_dist;

public:
  uniform_real_rand() noexcept{}
  uniform_real_rand(double  min, double  max) noexcept{reset(min,max);}

  void  reset(double  min, double  max) noexcept{m_dist = std::uniform_real_distribution<>(min,max);}

  double  operator()() noexcept{return m_dist(random_number_engine);}

};


class
string_form
{
  char  m_buffer[512] = {0};

  int  m_length=0;

public:
  string_form() noexcept{}
  string_form(const char*  fmt, ...) noexcept;

  const char*  operator()(const char*  fmt, ...) noexcept;
  const char*  operator()() const noexcept{return m_buffer;}

  int  size() const noexcept{return m_length;}

};


std::string  make_string_from_file(const char*  filepath) noexcept;




template<typename  T>
class
status_value
{
  T  m_data;

public:
  status_value(T  flag=0) noexcept: m_data(flag){}

  void  clear() noexcept{m_data = 0;}

  void  reverse(T  flag) noexcept
  {
      if(test(flag)){unset(flag);}
    else            {  set(flag);}
  }

  void    set(T  flag) noexcept{m_data |=  flag;}
  void  unset(T  flag) noexcept{m_data &= ~flag;}
  bool   test(T  flag) const noexcept{return m_data&flag;}

};




class
fixed_t
{
public:
  struct raw_value{  int  m_data;  explicit constexpr raw_value(int  v) noexcept: m_data(v){}};

private:
  static constexpr int  m_shift_amount = 16;

  int  m_data;

  static constexpr int  to_fpn(int  i) noexcept{return i<<m_shift_amount;}
  static constexpr int  to_int(int  i) noexcept{return i>>m_shift_amount;}
  static constexpr int  to_int(double  d) noexcept{return static_cast<int>(d);}

  static constexpr int  to_fract(double  d) noexcept{return to_int((d-to_int(d))*65536.0);}

  static constexpr int  to_fpn(double  d) noexcept{return (to_int(d)<<m_shift_amount)|to_fract(d);}

public:
  constexpr fixed_t(int  v=0)  noexcept: m_data(to_fpn(v)){}
  explicit constexpr fixed_t(raw_value  rv) noexcept: m_data(rv.m_data){}
  constexpr fixed_t(double  d) noexcept: m_data(to_fpn(d)){}

  constexpr operator int() const noexcept{return to_int(m_data);}

  constexpr int  get_raw_integer() const noexcept{return m_data;}

  constexpr bool  operator==(fixed_t  rhs) const noexcept{return m_data == rhs.m_data;}
  constexpr bool  operator!=(fixed_t  rhs) const noexcept{return m_data != rhs.m_data;}
  constexpr bool  operator< (fixed_t  rhs) const noexcept{return m_data <  rhs.m_data;}
  constexpr bool  operator<=(fixed_t  rhs) const noexcept{return m_data <= rhs.m_data;}
  constexpr bool  operator> (fixed_t  rhs) const noexcept{return m_data >  rhs.m_data;}
  constexpr bool  operator>=(fixed_t  rhs) const noexcept{return m_data >= rhs.m_data;}

  constexpr bool  operator==(int  i) const noexcept{return m_data == to_fpn(i);}
  constexpr bool  operator!=(int  i) const noexcept{return m_data != to_fpn(i);}
  constexpr bool  operator< (int  i) const noexcept{return m_data <  to_fpn(i);}
  constexpr bool  operator<=(int  i) const noexcept{return m_data <= to_fpn(i);}
  constexpr bool  operator> (int  i) const noexcept{return m_data >  to_fpn(i);}
  constexpr bool  operator>=(int  i) const noexcept{return m_data >= to_fpn(i);}

  fixed_t&  operator=(fixed_t  rhs) noexcept{  m_data = rhs.m_data;  return *this;}
  fixed_t&  operator=(int        i) noexcept{  m_data =  to_fpn(i);  return *this;}
  fixed_t&  operator=(double     d) noexcept{  m_data =  to_fpn(d);  return *this;}

  fixed_t&  operator+=(fixed_t  rhs) noexcept{  m_data += rhs.m_data;  return *this;}
  fixed_t&  operator-=(fixed_t  rhs) noexcept{  m_data -= rhs.m_data;  return *this;}

  fixed_t&  operator*=(int  i) noexcept{  m_data *= i;  return *this;}
  fixed_t&  operator/=(int  i) noexcept{  m_data /= i;  return *this;}
  fixed_t&  operator%=(int  i) noexcept{  m_data %= i;  return *this;}

  constexpr fixed_t  operator+(fixed_t  rhs) const noexcept{return fixed_t(raw_value(m_data+rhs.m_data));}
  constexpr fixed_t  operator-(fixed_t  rhs) const noexcept{return fixed_t(raw_value(m_data-rhs.m_data));}

  constexpr fixed_t  operator+(int  i) const noexcept{return fixed_t(raw_value( m_data+to_fpn(i)));}
  constexpr fixed_t  operator-(int  i) const noexcept{return fixed_t(raw_value( m_data-to_fpn(i)));}
  constexpr fixed_t  operator*(int  i) const noexcept{return fixed_t(raw_value( m_data*i        ));}
  constexpr fixed_t  operator/(int  i) const noexcept{return fixed_t(raw_value( m_data/i        ));}
  constexpr fixed_t  operator%(int  i) const noexcept{return fixed_t(raw_value( m_data%i        ));}
  constexpr fixed_t  operator-(      ) const noexcept{return fixed_t(raw_value(-m_data          ));}

  constexpr fixed_t  abs() const noexcept{return (m_data < 0)? fixed_t(raw_value(-m_data)):*this;}

  void  print() const noexcept;

};


template<typename  T>
class
pointer_wrapper
{
  T*  m_data;

public:
  constexpr pointer_wrapper(T*  ptr=nullptr) noexcept: m_data(ptr){}

  template<typename  U>
  constexpr pointer_wrapper(U*  ptr) noexcept: m_data(static_cast<T*>(ptr)){}

  pointer_wrapper&  operator=(T*  ptr) noexcept{  m_data = ptr;  return *this;}

  template<typename  U>
  pointer_wrapper&  operator=(U*  ptr) noexcept{  m_data = static_cast<T*>(ptr);  return *this;}

  constexpr T*  operator->() const noexcept{return  m_data;}
  constexpr T&  operator *() const noexcept{return *m_data;}

  constexpr pointer_wrapper  operator+(int  n) const noexcept{return m_data+n;}
  constexpr pointer_wrapper  operator-(int  n) const noexcept{return m_data-n;}

  constexpr operator bool() const noexcept{return m_data;}

  constexpr bool  operator==(pointer_wrapper  ptr) const noexcept{return m_data == ptr.m_data;}
  constexpr bool  operator!=(pointer_wrapper  ptr) const noexcept{return m_data != ptr.m_data;}
  constexpr bool  operator< (pointer_wrapper  ptr) const noexcept{return m_data <  ptr.m_data;}
  constexpr bool  operator<=(pointer_wrapper  ptr) const noexcept{return m_data <= ptr.m_data;}
  constexpr bool  operator> (pointer_wrapper  ptr) const noexcept{return m_data >  ptr.m_data;}
  constexpr bool  operator>=(pointer_wrapper  ptr) const noexcept{return m_data >= ptr.m_data;}

  template<typename  U>constexpr bool  operator==(const U*  ptr) const noexcept{return m_data == static_cast<const T*>(ptr);}
  template<typename  U>constexpr bool  operator!=(const U*  ptr) const noexcept{return m_data != static_cast<const T*>(ptr);}
  template<typename  U>constexpr bool  operator< (const U*  ptr) const noexcept{return m_data <  static_cast<const T*>(ptr);}
  template<typename  U>constexpr bool  operator<=(const U*  ptr) const noexcept{return m_data <= static_cast<const T*>(ptr);}
  template<typename  U>constexpr bool  operator> (const U*  ptr) const noexcept{return m_data >  static_cast<const T*>(ptr);}
  template<typename  U>constexpr bool  operator>=(const U*  ptr) const noexcept{return m_data >= static_cast<const T*>(ptr);}

};


template<typename  T>
T
post_increment(T&  x, T  amount) noexcept
{
  auto  t = x          ;
            x += amount;

  return t;
}


template<typename  T>
class
post_incrementor
{
  T&  m_value;

public:
  post_incrementor(T&  v) noexcept: m_value(v){}

  T  operator()(int  amount) noexcept
  {
    auto  t = m_value          ;
              m_value += amount;

    return t;
  }

};


}




#endif




