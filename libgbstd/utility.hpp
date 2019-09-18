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




}




#endif




