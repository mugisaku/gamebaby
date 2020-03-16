#include"libgbstd/parser.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




token&
token::
assign(const token&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_begin = rhs.m_begin;
      m_end   =   rhs.m_end;
      m_kind  =  rhs.m_kind;

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
      case(kind::block):
          new(&m_data) token_block(rhs.m_data.blk);
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

      std::swap(m_begin,rhs.m_begin);
      std::swap(m_end  ,rhs.m_end  );
      std::swap(m_kind ,rhs.m_kind );

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
      case(kind::block):
          new(&m_data) token_block(std::move(rhs.m_data.blk));
          break;
        }
    }


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, uint64_t  n) noexcept
{
  clear();

  m_begin = begin;
  m_end   =   end;

  m_data.n = n;

  m_kind = kind::integer;


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, std::string&&  s, int  sym) noexcept
{
  clear();

  m_begin = begin;
  m_end   =   end;

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
assign(const char*  begin, const char*  end, double  f) noexcept
{
  clear();

  m_begin = begin;
  m_end   =   end;

  m_data.f = f;

  m_kind = kind::floating_point_number;


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, operator_code  opco) noexcept
{
  clear();

  m_begin = begin;
  m_end   =   end;

  m_data.opco = opco;

  m_kind = kind::operator_code;


  return *this;
}


token&
token::
assign(const char*  begin, const char*  end, token_block&&  blk) noexcept
{
  clear();

  m_begin = begin;
  m_end   =   end;

  new(&m_data) token_block(std::move(blk));

  m_kind = kind::block;


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
  case(kind::block):
      m_data.blk.~token_block();
      break;
    }


  m_begin = nullptr;
  m_end   = nullptr;

  m_kind = kind::null;
}




bool
token::
is_block(operator_code  open, operator_code  close) const noexcept
{
  return (m_kind == kind::block) && m_data.blk.test(open,close);
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
print(const char*  base, int  indent) const noexcept
{
  int  i = 0;

    if(base)
    {
      auto  p = base;

        while(p < m_begin)
        {
           if(*p == '\n')
           {
             ++i;
           }


          ++p;
        }


      printf("%4d  ",i);
    }


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
  case(kind::block): m_data.blk.print(base,indent+2);break;
  default: printf("UNKNOWN");break;
    }
}


void
token::
print_source(const char*  base) const noexcept
{
    if(base)
    {
      int  n = 1;

        while(base < m_begin)
        {
            if(*base == '\n')
            {
              ++n;
            }


          ++base;
        }


      printf("%4d ",n);
    }


  auto  p = m_begin;

    while(p < m_end)
    {
      printf("%c",*p++);
    }


  printf("\n");
}




}




