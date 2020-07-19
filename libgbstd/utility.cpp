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




std::string
unique_string::
operator+(std::string_view  sv) const noexcept
{
  std::string  s(std::string_view(*this));

  s += sv;

  return std::move(s);
}


unique_string&
unique_string::
assign(unique_string&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_data,rhs.m_data);
      std::swap(m_size,rhs.m_size);
    }


  return *this;
}


unique_string&
unique_string::
assign(std::string_view  sv) noexcept
{
  auto  new_sz = sv.size();

    if(m_size < new_sz)
    {
      free(m_data)                                      ;
           m_data = static_cast<char*>(malloc(new_sz+1));
    }


  m_size = new_sz;


  auto  dst =    m_data;
  auto  src = sv.data();

  size_t  n = new_sz;

    while(n--)
    {
      *dst++ = *src++;
    }


  *dst = 0;

  return *this;
}


void
unique_string::
clear() noexcept
{
    if(m_data)
    {
      free(m_data)         ;
           m_data = nullptr;
    }


  m_size = 0;
}


char*
unique_string::
release() noexcept
{
  auto  p = m_data          ;
            m_data = nullptr;

  m_size = 0;

  return p;
}




shared_string&
shared_string::
assign(const shared_string&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++reinterpret_cast<size_t&>(*m_data);
        }
    }


  return *this;
}


shared_string&
shared_string::
assign(shared_string&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


shared_string&
shared_string::
assign(std::string_view  sv) noexcept
{
  auto  new_sz = sv.size();

    if(size() < new_sz)
    {
      auto  alloc_sz = new_sz+sizeof(size_t)+sizeof(size_t)+1;

      free(m_data)                                      ;
           m_data = static_cast<char*>(malloc(alloc_sz));
    }


  reinterpret_cast<size_t&>(m_data                ) =      1;
  reinterpret_cast<size_t&>(m_data[sizeof(size_t)]) = new_sz;

  auto  dst = &m_data[sizeof(size_t)*2];
  auto  src = sv.data();

  size_t  n = new_sz;

    while(n--)
    {
      *dst++ = *src++;
    }


  *dst = 0;

  return *this;
}


void
shared_string::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--reinterpret_cast<size_t&>(m_data))
        {
          free(m_data);
        }


      m_data = nullptr;
    }
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




