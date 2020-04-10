#ifndef gbstd_misc_hpp_was_included
#define gbstd_misc_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<cstdint>
#include<stdexcept>
#include<memory>
#include<new>




namespace gbstd{




template<typename  T>
std::unique_ptr<T>
clone_unique(const std::unique_ptr<T>&  p) noexcept
{
  return std::make_unique<T>(p? *p:nullptr);
}


template<typename  T>
class
clone_ptr
{
  T*  m_data=nullptr;

public:
  constexpr clone_ptr() noexcept{}
  constexpr clone_ptr(T*  p) noexcept: m_data(p){}

  template<typename  U>
  constexpr clone_ptr(U*  p) noexcept: m_data(static_cast<T*>(p)){}

  clone_ptr(const clone_ptr&   rhs) noexcept{assign(rhs);}
  clone_ptr(      clone_ptr&&  rhs) noexcept{assign(std::move(rhs));}
  ~clone_ptr(){clear();}

  clone_ptr&  operator=(const clone_ptr&   rhs) noexcept{return assign(rhs);}
  clone_ptr&  operator=(      clone_ptr&&  rhs) noexcept{return assign(std::move(rhs));}

  constexpr T&  operator*()  const noexcept{return *m_data;}
  constexpr T*  operator->() const noexcept{return  m_data;}

  constexpr operator bool() const noexcept{return m_data;}

  clone_ptr&  assign(const clone_ptr&   rhs) noexcept{
      if(this != &rhs){
        clear();
          if(rhs.m_data){
            m_data = new T(*rhs.m_data);
          }
      }
    return *this;
  }

  clone_ptr&  assign(clone_ptr&&  rhs) noexcept{
      if(this != &rhs){
        clear();
        std::swap(m_data,rhs.m_data);
      }
    return *this;
  }

  void  clear(){  delete m_data;  m_data = nullptr;}

  constexpr T*  get() const noexcept{return m_data;}

};


template<typename  T, typename...  Args>
inline clone_ptr<T>
make_clone(Args&&...  args) noexcept
{
  return clone_ptr<T>(new T(std::forward<Args>(args)...));
}


template<typename  T>inline void  create_at(void*  dst, const T&   src) noexcept{new(dst) T(src);}
template<typename  T>inline void  create_at(void*  dst,       T&&  src) noexcept{new(dst) T(std::forward<T>(src));}


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

  constexpr operator const T*() const noexcept{return m_data;}

  constexpr T*  operator->() const noexcept{return  m_data;}
  constexpr T&  operator *() const noexcept{return *m_data;}

  constexpr T&  operator[](int  i) const noexcept{return m_data[i];}

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
inline pointer_wrapper<T>
wrap(T*  p) noexcept
{
  return pointer_wrapper(p);
}


template<typename  T>
inline pointer_wrapper<T>
wrap(const std::unique_ptr<T>&  p) noexcept
{
  return pointer_wrapper(p.get());
}


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


class    canvas;
class execution;


class
callback_wrapper
{
  void*  m_userdata=nullptr;

  union{
    void  (     *fnptr)();
    void  (  *cv_fnptr)(const canvas&);
    void  (*exec_fnptr)(execution&);
    void  (dummy::*membfnptr)();
    void  (dummy::*cv_membfnptr)(const canvas&);
    void  (dummy::*exec_membfnptr)(execution&);
  } m_callback;

public:
  callback_wrapper() noexcept{m_callback.fnptr = nullptr;}

  template<typename  T>
  callback_wrapper(T&  t, void  (T::*cb)()) noexcept{assign(t,cb);}

  template<typename  T>
  callback_wrapper(T&  t, void  (T::*cb)(const canvas&)) noexcept{assign(t,cb);}

  template<typename  T>
  callback_wrapper(T&  t, void  (T::*cb)(execution&)) noexcept{assign(t,cb);}

  callback_wrapper(void  (*cb)(             )) noexcept{assign(cb);}
  callback_wrapper(void  (*cb)(const canvas&)) noexcept{assign(cb);}
  callback_wrapper(void  (*cb)(execution&)) noexcept{assign(cb);}


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

  callback_wrapper&  assign(void(*cb)(execution&)) noexcept
  {
    m_userdata            = nullptr;
    m_callback.exec_fnptr =      cb;

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

  template<typename  T>
  callback_wrapper&  assign(T&  t, void(T::*cb)(execution&)) noexcept
  {
    m_userdata                =                                               &t;
    m_callback.exec_membfnptr = reinterpret_cast<void(dummy::*)(execution&)>(cb);

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

  void  operator()(execution&  exec) const noexcept
  {
         if(m_userdata           ){(reinterpret_cast<dummy*>(m_userdata)->*m_callback.exec_membfnptr)(exec);}
    else if(m_callback.exec_fnptr){m_callback.exec_fnptr(exec);}
  }

};




template<typename  T>
class
list_node_header
{
  T*  m_previous;
  T*  m_next    ;

};




}




#endif




