#include"libgbstd/parser.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




const token
token::
null;


token&
token::
assign(const token&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_begin       = rhs.m_begin;
      m_end         = rhs.m_end;
      m_line_number = rhs.m_line_number;
      m_kind        = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::floating_point_number):
          m_data.f = rhs.m_data.f;
          break;
      case(kind::operator_code):
          m_data.opco = rhs.m_data.opco;
          break;
      case(kind::integer):
          m_data.n = rhs.m_data.n;
          break;
      case(kind::identifier):
      case(kind::single_quoted):
      case(kind::double_quoted):
          new(&m_data) std::string(rhs.m_data.s);
          break;
        }
    }


  return *this;
}


token&
token::
assign(token&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_begin      ,rhs.m_begin      );
      std::swap(m_end        ,rhs.m_end        );
      std::swap(m_line_number,rhs.m_line_number);
      std::swap(m_kind       ,rhs.m_kind       );

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::floating_point_number):
          m_data.f = rhs.m_data.f;
          break;
      case(kind::operator_code):
          m_data.opco = rhs.m_data.opco;
          break;
      case(kind::integer):
          m_data.n = rhs.m_data.n;
          break;
      case(kind::identifier):
      case(kind::single_quoted):
      case(kind::double_quoted):
          new(&m_data) std::string(std::move(rhs.m_data.s));
          break;
        }
    }


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, int  ln, uint64_t  n) noexcept
{
  clear();

  m_begin       = begin;
  m_end         =   end;
  m_line_number =    ln;

  m_data.n = n;

  m_kind = kind::integer;


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, int  ln, std::string&&  s, int  sym) noexcept
{
  clear();

  m_begin       = begin;
  m_end         =   end;
  m_line_number =    ln;

  new(&m_data) std::string(std::move(s));

    switch(sym)
    {
  case('\''): m_kind = kind::single_quoted;break;
  case('\"'): m_kind = kind::double_quoted;break;
  case('\0'): m_kind = kind::identifier   ;break;
    }


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, int  ln, double  f) noexcept
{
  clear();

  m_begin       = begin;
  m_end         =   end;
  m_line_number =    ln;

  m_data.f = f;

  m_kind = kind::floating_point_number;


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, int  ln, operator_code  opco) noexcept
{
  clear();

  m_begin       = begin;
  m_end         =   end;
  m_line_number =    ln;

  m_data.opco = opco;

  m_kind = kind::operator_code;


  return *this;
}




void
token::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null):
  case(kind::integer):
  case(kind::floating_point_number):
  case(kind::operator_code):
      break;
  case(kind::identifier):
  case(kind::single_quoted):
  case(kind::double_quoted):
      m_data.s.~basic_string();
      break;
    }


  m_begin = nullptr;
  m_end   = nullptr;

  m_kind = kind::null;
}




void
token::
print_string(const std::string&  s, char  punct) noexcept
{
  printf("%c",punct);

    for(auto  c: s)
    {
           if(c == '\0'){printf("\\0");}
      else if(c == '\n'){printf("\\n");}
      else if(c == '\t'){printf("\\t");}
      else if(c == '\r'){printf("\\r");}
      else if(c == '\b'){printf("\\b");}
      else if(c == '\a'){printf("\\a");}
      else if(c == '\v'){printf("\\v");}
      else if(c == '\f'){printf("\\f");}
      else if(c == '\?'){printf("\\?");}
      else if(c == '\\'){printf("\\\\");}
      else if(c == '\''){printf("\\\'");}
      else if(c == '\"'){printf("\\\"");}
      else if(isascii(c)){printf("%c",c);}
      else               {printf("\\x%x",c);}
    }


  printf("%c",punct);
}


void
token::
print(int  indent) const noexcept
{
  printf("%4d: ",m_line_number);

    for(int  n = 0;  n < indent;  ++n)
    {
      printf(" ");
    }


    switch(m_kind)
    {
  case(kind::null): printf("NULL");break;
  case(kind::integer): printf("%" PRIu64,m_data.n);break;
  case(kind::identifier): printf("%s ",m_data.s.data());break;
  case(kind::single_quoted): print_string(m_data.s,'\'');break;
  case(kind::double_quoted): print_string(m_data.s,'\"');break;
  case(kind::floating_point_number): printf("%f",m_data.f);break;
  case(kind::operator_code): printf("%s",m_data.opco.get_string());break;
  default: printf("UNKNOWN");break;
    }
}




}




