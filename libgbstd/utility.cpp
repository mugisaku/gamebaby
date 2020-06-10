#include"libgbstd/utility.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{


int  reporting_counter;


void
printf_with_indent(int  indent, const char*  fmt, ...) noexcept
{
  char  buf[256];

  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  const char*  p = buf;

    while(*p)
    {
      fputc(*p,stdout);

        if(*p++ == '\n')
        {
            for(int  n = 0;  n < indent;  ++n)
            {
              fputc(' ',stdout);
            }
        }
    }
}




bool
is_png(const void*  ptr) noexcept
{
  static const uint8_t  sig[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};

  return std::memcmp(ptr,sig,sizeof(sig)) == 0;
}


bool
is_webp(const void*  ptr) noexcept
{
  auto  p = static_cast<const char*>(ptr);

  return (std::memcmp(p  ,"RIFF",4) == 0) &&
         (std::memcmp(p+8,"WEBP",4) == 0);
}


namespace{
std::vector<std::string>
strings;
}


void
clear_formed_strings() noexcept
{
  strings.clear();
}


std::string_view
form_string(const char*  fmt, ...) noexcept
{
  static char  buf[1024];

  va_list  ap;

  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);


  return strings.emplace_back(buf);
}


std::string_view
form_string(size_t  n, const char*  fmt, ...) noexcept
{
  auto  p = static_cast<char*>(malloc(n));

  va_list  ap;

  va_start(ap,fmt);

  vsnprintf(p,n,fmt,ap);

  va_end(ap);

  strings.emplace_back(p);

  delete[] p;

  return strings.back();
}


int
get_number_of_newlines(const char*  begin, const char*  end) noexcept
{
  int  n = 0;

    while(begin < end)
    {
        if(*begin++ == '\n')
        {
          ++n;
        }
    }


  return n;
}



}




