#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_element&
syntax_element::
assign(const syntax_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_separator = rhs.m_separator;
      m_kind      = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::string    ): new(&m_data) std::string(rhs.m_data.str);break;
      case(kind::group     ): new(&m_data) syntax_group(rhs.m_data.grp);break;
      case(kind::definition): m_data.def = rhs.m_data.def;break;
        }
    }


  return *this;
}


syntax_element&
syntax_element::
assign(syntax_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_separator,rhs.m_separator);
      std::swap(m_kind     ,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::string    ): new(&m_data) std::string(std::move(rhs.m_data.str));break;
      case(kind::group     ): new(&m_data) syntax_group(std::move(rhs.m_data.grp));break;
      case(kind::definition): std::swap(m_data.def,rhs.m_data.def);break;
        }
    }


  return *this;
}


syntax_element&
syntax_element::
assign(int  sep, std::string_view  sv) noexcept
{
  clear();

  m_separator = sep;

  new(&m_data) std::string(sv);

  m_kind = kind::string;

  return *this;
}


syntax_element&
syntax_element::
assign(int  sep, std::string&&  s) noexcept
{
  clear();

  m_separator = sep;

  m_kind = kind::string;

  new(&m_data) std::string(std::move(s));

  return *this;
}


syntax_element&
syntax_element::
assign(int  sep, syntax_group&&  grp) noexcept
{
  clear();

  m_separator = sep;

  new(&m_data) syntax_group(std::move(grp));

  m_kind = kind::group;

  return *this;
}


syntax_element&
syntax_element::
assign(int  sep, const syntax_definition&  def) noexcept
{
  clear();

  m_separator = sep;

  m_data.def = &def;

  m_kind = kind::definition;

  return *this;
}




void
syntax_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null  ):;break;
  case(kind::string): std::destroy_at(&m_data.str);break;
  case(kind::group): std::destroy_at(&m_data.grp);break;
  case(kind::definition): ;break;
    }


  m_kind = kind::null;
}


void
syntax_element::
print() const noexcept
{
    if(m_separator)
    {
      printf(" %c ",m_separator);
    }


    switch(m_kind)
    {
  case(kind::null  ):;break;
  case(kind::string): printf("\"%s\"",m_data.str.data());break;
  case(kind::group): m_data.grp.print();break;
  case(kind::definition): printf("%s",m_data.def->get_name().data());break;
    }
}


}




