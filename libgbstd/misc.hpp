#ifndef gbstd_misc_hpp_was_included
#define gbstd_misc_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstdint>
#include<stdexcept>




namespace gbstd{




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




class dummy{};




class
validity
{
  bool  m_value=true;

public:
  operator bool() const noexcept{return m_value;}

  void   enable() noexcept{m_value =  true;}
  void  disable() noexcept{m_value = false;}

};


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




}




#endif




