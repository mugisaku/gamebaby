#ifndef gbstd_misc_hpp_was_included
#define gbstd_misc_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstring>
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

  operator bool() const noexcept{return m_data;}

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


class canvas;


class
callback_wrapper
{
  void*  m_userdata=nullptr;

  union{
    void  (       *fnptr)();
    void  (       *cv_fnptr)(const canvas&);
    void  (dummy::*membfnptr)();
    void  (dummy::*cv_membfnptr)(const canvas&);
  } m_callback;

public:
  callback_wrapper() noexcept{m_callback.fnptr = nullptr;}

  template<typename  T>
  callback_wrapper(T&  t, void  (T::*cb)()) noexcept{assign(t,cb);}

  template<typename  T>
  callback_wrapper(T&  t, void  (T::*cb)(const canvas&)) noexcept{assign(t,cb);}

  callback_wrapper(void  (*cb)(             )) noexcept{assign(cb);}
  callback_wrapper(void  (*cb)(const canvas&)) noexcept{assign(cb);}


  callback_wrapper&  operator=(void  (*cb)(             )) noexcept {return assign(cb);}
  callback_wrapper&  operator=(void  (*cb)(const canvas&)) noexcept {return assign(cb);}


  callback_wrapper&  assign(void(*cb)()) noexcept
  {
    m_userdata       = nullptr;
    m_callback.fnptr =      cb;

    return *this;
  }

  callback_wrapper&  assign(void(*cb)(const canvas&)) noexcept
  {
    m_userdata          = nullptr;
    m_callback.cv_fnptr =      cb;

    return *this;
  }

  template<typename  T>
  callback_wrapper&  assign(T&  t, void(T::*cb)()) noexcept
  {
    m_userdata           =                                     &t;
    m_callback.membfnptr = reinterpret_cast<void(dummy::*)()>(cb);

    return *this;
  }

  template<typename  T>
  callback_wrapper&  assign(T&  t, void(T::*cb)(const canvas&)) noexcept
  {
    m_userdata              =                                                  &t;
    m_callback.cv_membfnptr = reinterpret_cast<void(dummy::*)(const canvas&)>(cb);

    return *this;
  }


  void  operator()() const noexcept
  {
         if(m_userdata      ){(reinterpret_cast<dummy*>(m_userdata)->*m_callback.membfnptr)();}
    else if(m_callback.fnptr){m_callback.fnptr();}
  }

  void  operator()(const canvas&  cv) const noexcept
  {
         if(m_userdata         ){(reinterpret_cast<dummy*>(m_userdata)->*m_callback.cv_membfnptr)(cv);}
    else if(m_callback.cv_fnptr){m_callback.cv_fnptr(cv);}
  }

};




}




#endif




