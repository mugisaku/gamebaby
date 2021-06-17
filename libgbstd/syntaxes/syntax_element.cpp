#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_element&
syntax_element::
assign(const syntax_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::keyword  ):
      case(kind::string   ):
      case(kind::reference): new(&m_data) std::u16string(rhs.m_data.s);break;

      case(kind::formula         ):
      case(kind::optional_formula):
      case(kind::multiple_formula): new(&m_data) syntax_formula(rhs.m_data.f);break;
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

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::keyword  ):
      case(kind::string   ):
      case(kind::reference): new(&m_data) std::u16string(std::move(rhs.m_data.s));break;

      case(kind::formula         ):
      case(kind::optional_formula):
      case(kind::multiple_formula): new(&m_data) syntax_formula(std::move(rhs.m_data.f));break;
        }
    }


  return *this;
}


syntax_element&
syntax_element::
assign(std::u16string_view  sv) noexcept
{
  clear();

       if(sv == u"integer_literal" ){m_kind = kind::integer_literal;}
  else if(sv == u"floating_literal"){m_kind = kind::floating_literal;}
  else if(sv == u"string_literal"  ){m_kind = kind::string_literal;}
  else if(sv == u"identifier"      ){m_kind = kind::identifier;}
  else
    {
      new(&m_data) std::u16string(sv);

      m_kind = kind::string;
    }


  return *this;
}


syntax_element&
syntax_element::
assign(std::u16string&&  s) noexcept
{
  clear();

  m_kind = kind::string;

  new(&m_data) std::u16string(std::move(s));

  return *this;
}


syntax_element&
syntax_element::
assign(syntax_keyword&&  kw) noexcept
{
  clear();

  m_kind = kind::keyword;

  new(&m_data) std::u16string(std::move(kw.string()));

  return *this;
}


syntax_element&
syntax_element::
assign(syntax_formula&&  f) noexcept
{
  clear();

  new(&m_data) syntax_formula(std::move(f));

  m_kind = kind::formula;

  return *this;
}


syntax_element&
syntax_element::
assign(syntax_optional_formula&&  f) noexcept
{
  clear();

  new(&m_data) syntax_formula(std::move(f.formula()));

  m_kind = kind::optional_formula;

  return *this;
}


syntax_element&
syntax_element::
assign(syntax_multiple_formula&&  f) noexcept
{
  clear();

  new(&m_data) syntax_formula(std::move(f.formula()));

  m_kind = kind::multiple_formula;

  return *this;
}


syntax_element&
syntax_element::
assign(syntax_reference&&  ref) noexcept
{
  clear();

  new(&m_data) std::u16string(std::move(ref.string()));

  m_kind = kind::reference;

  return *this;
}




void
syntax_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::keyword  ):
  case(kind::string   ):
  case(kind::reference): std::destroy_at(&m_data.s);break;

  case(kind::formula         ):
  case(kind::optional_formula):
  case(kind::multiple_formula): std::destroy_at(&m_data.f);break;
    }


  m_kind = kind::null;
}


void
syntax_element::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null            ): printf("<null>");break;
  case(kind::integer_literal ): printf("integer_literal");break;
  case(kind::floating_literal): printf("floating_literal");break;
  case(kind::string_literal  ): printf("string_literal");break;
  case(kind::identifier      ): printf("identifier");break;
  case(kind::keyword):
      printf("<");
      gbstd::print(m_data.s);
      printf(">");
      break;
  case(kind::string):
      printf("\"");
      gbstd::print(m_data.s);
      printf("\"");
      break;
  case(kind::formula):
      printf("(");
      m_data.f.print();
      printf(")");
      break;
  case(kind::optional_formula):
      printf("[");
      m_data.f.print();
      printf("]");
      break;
  case(kind::multiple_formula):
      printf("{");
      m_data.f.print();
      printf("}");
      break;
  case(kind::reference): gbstd::print(m_data.s);break;
    }
}


}




