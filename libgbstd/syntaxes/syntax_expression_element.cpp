#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_expression_element&
syntax_expression_element::
assign(const syntax_expression_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::keyword   ): new(&m_data) std::u16string(rhs.m_data.str);break;
      case(kind::string    ): new(&m_data) std::u16string(rhs.m_data.str);break;
      case(kind::expression): new(&m_data) syntax_expression(rhs.m_data.expr);break;
      case(kind::optional_expression): new(&m_data) syntax_expression(rhs.m_data.expr);break;
      case(kind::multiple_expression): new(&m_data) syntax_expression(rhs.m_data.expr);break;
      case(kind::definition): m_data.def = rhs.m_data.def;break;
        }
    }


  return *this;
}


syntax_expression_element&
syntax_expression_element::
assign(syntax_expression_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::keyword   ): new(&m_data) std::u16string(std::move(rhs.m_data.str));break;
      case(kind::string    ): new(&m_data) std::u16string(std::move(rhs.m_data.str));break;
      case(kind::expression): new(&m_data) syntax_expression(std::move(rhs.m_data.expr));break;
      case(kind::optional_expression): new(&m_data) syntax_expression(std::move(rhs.m_data.expr));break;
      case(kind::multiple_expression): new(&m_data) syntax_expression(std::move(rhs.m_data.expr));break;
      case(kind::definition): std::swap(m_data.def,rhs.m_data.def);break;
        }
    }


  return *this;
}


syntax_expression_element&
syntax_expression_element::
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


syntax_expression_element&
syntax_expression_element::
assign(std::u16string&&  s) noexcept
{
  clear();

  m_kind = kind::string;

  new(&m_data) std::u16string(std::move(s));

  return *this;
}


syntax_expression_element&
syntax_expression_element::
assign(syntax_keyword&&  kw) noexcept
{
  clear();

  m_kind = kind::keyword;

  new(&m_data) std::u16string(std::move(kw.get_string()));

  return *this;
}


syntax_expression_element&
syntax_expression_element::
assign(syntax_expression&&  expr) noexcept
{
  clear();

  new(&m_data) syntax_expression(std::move(expr));

  m_kind = kind::expression;

  return *this;
}


syntax_expression_element&
syntax_expression_element::
assign(syntax_optional_expression&&  expr) noexcept
{
  clear();

  new(&m_data) syntax_expression(std::move(expr.get_expression()));

  m_kind = kind::optional_expression;

  return *this;
}


syntax_expression_element&
syntax_expression_element::
assign(syntax_multiple_expression&&  expr) noexcept
{
  clear();

  new(&m_data) syntax_expression(std::move(expr.get_expression()));

  m_kind = kind::multiple_expression;

  return *this;
}


syntax_expression_element&
syntax_expression_element::
assign(const syntax_definition&  def) noexcept
{
  clear();

  m_data.def = &def;

  m_kind = kind::definition;

  return *this;
}




void
syntax_expression_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::keyword): std::destroy_at(&m_data.str);break;
  case(kind::string): std::destroy_at(&m_data.str);break;
  case(kind::expression): std::destroy_at(&m_data.expr);break;
  case(kind::optional_expression): std::destroy_at(&m_data.expr);break;
  case(kind::multiple_expression): std::destroy_at(&m_data.expr);break;
  case(kind::definition): ;break;
    }


  m_kind = kind::null;
}


void
syntax_expression_element::
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
      gbstd::print(m_data.str);
      printf(">");
      break;
  case(kind::string):
      printf("\"");
      gbstd::print(m_data.str);
      printf("\"");
      break;
  case(kind::expression):
      printf("(");
      m_data.expr.print();
      printf(")");
      break;
  case(kind::optional_expression):
      printf("[");
      m_data.expr.print();
      printf("]");
      break;
  case(kind::multiple_expression):
      printf("{");
      m_data.expr.print();
      printf("}");
      break;
  case(kind::definition): gbstd::print(m_data.def->get_name());break;
    }
}


}




