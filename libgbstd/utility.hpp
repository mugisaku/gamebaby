#ifndef utility_hpp_was_included
#define utility_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstdint>
#include<string_view>
#include<random>
#include<stdexcept>




#ifndef report
#define report printf("[report %8d] %s, %s, %d\n",gbstd::reporting_counter++,__FILE__,__func__,__LINE__);
#endif




namespace gbstd{


extern int  reporting_counter;


inline void  printnl() noexcept{printf("\n");}


template<typename  T, typename...  Args>
inline T*
create_at(T*  t, Args&&...  args) noexcept
{
  return new(t) T(std::forward<Args>(args)...);
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

void  clear_formed_strings() noexcept;
std::string_view  form_string(           const char*  fmt, ...) noexcept;
std::string_view  form_string(size_t  n, const char*  fmt, ...) noexcept;


class
unique_string
{
  char*  m_data=nullptr;

  size_t  m_size=0;

public:
  unique_string(const unique_string&) noexcept=delete;
  unique_string(      unique_string&&  rhs) noexcept{assign(std::move(rhs));}

  unique_string() noexcept{}
  unique_string(std::string_view  sv) noexcept{assign(sv);}
 ~unique_string(){clear();}

  unique_string&  operator=(const unique_string&) noexcept=delete;
  unique_string&  operator=(unique_string&&  rhs) noexcept{return assign(std::move(rhs));}
  unique_string&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  bool  operator==(std::string_view  sv) const noexcept{return sv == static_cast<std::string_view>(*this);}

  operator std::string_view() const noexcept{return std::string_view(data(),size());}

  const char&  operator[](int  i) const noexcept{return data()[i];}

  std::string  operator+(std::string_view  sv) const noexcept;

  unique_string&  assign(unique_string&&  rhs) noexcept;
  unique_string&  assign(std::string_view  sv) noexcept;

  void  clear() noexcept;

  const char*  data() const noexcept{return m_data;}

  size_t  size() const noexcept{return m_size;}

  char*  release() noexcept;

};


class
shared_string
{
  char*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  shared_string(const shared_string&   rhs) noexcept{assign(rhs);}
  shared_string(      shared_string&&  rhs) noexcept{assign(std::move(rhs));}

  shared_string() noexcept{}
  shared_string(std::string_view  sv) noexcept{assign(sv);}
 ~shared_string(){unrefer();}

  shared_string&  operator=(const shared_string&   rhs) noexcept{return assign(rhs);}
  shared_string&  operator=(      shared_string&&  rhs) noexcept{return assign(std::move(rhs));}
  shared_string&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  bool  operator==(std::string_view  sv) const noexcept{return sv == static_cast<std::string_view>(*this);}

  operator std::string_view() const noexcept{return std::string_view(data(),size());}

  const char&  operator[](int  i) const noexcept{return data()[i];}

  shared_string&  assign(const shared_string&   rhs) noexcept;
  shared_string&  assign(      shared_string&&  rhs) noexcept;
  shared_string&  assign(std::string_view  sv) noexcept;

  const char*  data() const noexcept{return &m_data[sizeof(size_t)*2];}

  size_t  size() const noexcept{return m_data? reinterpret_cast<const size_t&>(m_data[sizeof(size_t)]):0;}

};


int  get_number_of_newlines(const char*  begin, const char*  end) noexcept;

}




#endif




