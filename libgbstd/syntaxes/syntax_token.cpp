#include"libgbstd/syntax.hpp"
#include<cinttypes>




namespace gbstd{




syntax_token&
syntax_token::
assign(const syntax_token&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

      m_x_position = rhs.m_x_position;
      m_y_position = rhs.m_y_position;

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::floating):
          m_data.f = rhs.m_data.f;
          break;
      case(kind::integer):
      case(kind::others):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::identifier):
      case(kind::string):
          new(&m_data) std::u16string(rhs.m_data.s);
          break;
        }
    }


  return *this;
}


syntax_token&
syntax_token::
assign(syntax_token&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

      std::swap(m_x_position,rhs.m_x_position);
      std::swap(m_y_position,rhs.m_y_position);

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::floating):
          m_data.f = rhs.m_data.f;
          break;
      case(kind::integer):
      case(kind::others):
          m_data.i = rhs.m_data.i;
          break;
      case(kind::identifier):
      case(kind::string):
          new(&m_data) std::u16string(std::move(rhs.m_data.s));
          break;
        }
    }


  return *this;
}


syntax_token&
syntax_token::
assign(uint64_t  i) noexcept
{
  clear();

  m_data.i = i;

  m_kind = kind::integer;


  return *this;
}


syntax_token&
syntax_token::
assign(std::u16string&&  s) noexcept
{
  clear();

  new(&m_data) std::u16string(std::move(s));

  m_kind = kind::string;


  return *this;
}


syntax_token&
syntax_token::
assign(syntax_identifier&&  id) noexcept
{
  clear();

  new(&m_data) std::u16string(std::move(id.string()));

  m_kind = kind::identifier;


  return *this;
}


syntax_token&
syntax_token::
assign(double  f) noexcept
{
  clear();

  m_data.f = f;

  m_kind = kind::floating;


  return *this;
}


syntax_token&
syntax_token::
assign(char16_t  c) noexcept
{
  clear();

  m_data.i = c;

  m_kind = kind::others;


  return *this;
}




void
syntax_token::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null):
  case(kind::integer):
  case(kind::floating):
  case(kind::others):
      break;
  case(kind::identifier):
  case(kind::string):
      std::destroy_at(&m_data.s);
      break;
    }


  m_kind = kind::null;

  m_x_position = 0;
  m_y_position = 0;
}


void
syntax_token::
set_position(int  x, int  y) noexcept
{
  m_x_position = x;
  m_y_position = y;
}


void
syntax_token::
print() const noexcept
{
    if(!*this)
    {
      return;
    }


  utf8_encoder  u8enc;

  printf("[%4d,%4d]: ",1+m_y_position,1+m_x_position);

    switch(m_kind)
    {
  case(kind::null      ): printf("<null>");break;
  case(kind::integer   ): printf("%" PRIu64,m_data.i);break;
  case(kind::floating  ): printf("%f",m_data.f);break;
  case(kind::others    ): printf("%s",u8enc(m_data.i).codes);break;
  case(kind::identifier):
      gbstd::print(m_data.s);
      break;
  case(kind::string):
      printf("\"");

      gbstd::print(m_data.s);

      printf("\"");
      break;
    }
}




}




