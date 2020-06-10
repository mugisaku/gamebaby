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

int  get_number_of_newlines(const char*  begin, const char*  end) noexcept;

}




#endif




