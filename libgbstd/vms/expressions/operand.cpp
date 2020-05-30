#include"libgbstd/vms/expression.hpp"




namespace gbstd{




operand&
operand::
assign(const operand&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kinds::boolean): create_at(&m_data.b,rhs.m_data.b);break;
      case(kinds::string ): create_at(&m_data.s,rhs.m_data.s);break;
      case(kinds::integer): create_at(&m_data.i,rhs.m_data.i);break;
      case(kinds::fpn    ): create_at(&m_data.f,rhs.m_data.f);break;
      case(kinds::identifier): create_at(&m_data.s,rhs.m_data.s);break;
      case(kinds::expression): create_at(&m_data.expr,rhs.m_data.expr);break;
        }
    }


  return *this;
}


operand&
operand::
assign(operand&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kinds::boolean): create_at(&m_data.b,std::move(rhs.m_data.b));break;
      case(kinds::string ): create_at(&m_data.s,std::move(rhs.m_data.s));break;
      case(kinds::integer): create_at(&m_data.i,std::move(rhs.m_data.i));break;
      case(kinds::fpn    ): create_at(&m_data.f,std::move(rhs.m_data.f));break;
      case(kinds::identifier): create_at(&m_data.s,std::move(rhs.m_data.s));break;
      case(kinds::expression): create_at(&m_data.expr,std::move(rhs.m_data.expr));break;
        }
    }


  return *this;
}


operand&
operand::
assign(nullptr_t) noexcept
{
  clear();

  m_kind = kinds::null_pointer;

  return *this;
}


operand&
operand::
assign(bool  b) noexcept
{
  clear();

  create_at(&m_data.b,b);

  m_kind = kinds::boolean;

  return *this;
}


operand&
operand::
assign(std::string_view  sv) noexcept
{
  clear();

  create_at(&m_data.s,sv);

  m_kind = kinds::string;

  return *this;
}


operand&
operand::
assign(int64_t  i) noexcept
{
  clear();

  create_at(&m_data.i,i);

  m_kind = kinds::integer;

  return *this;
}


operand&
operand::
assign(double  f) noexcept
{
  clear();

  create_at(&m_data.f,f);

  m_kind = kinds::fpn;

  return *this;
}


operand&
operand::
assign(identifier&&  id) noexcept
{
  clear();

  create_at(&m_data.s,id.release());

  m_kind = kinds::identifier;

  return *this;
}


operand&
operand::
assign(expression&&  e) noexcept
{
  clear();

  create_at(&m_data.expr,make_clone<expression>(std::move(e)));

  m_kind = kinds::expression;

  return *this;
}


operand&
operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kinds::null        ): break;
  case(kinds::null_pointer): break;
  case(kinds::boolean     ): break;
  case(kinds::string      ): std::destroy_at(&m_data.s);break;
  case(kinds::integer     ): std::destroy_at(&m_data.i);break;
  case(kinds::fpn         ): std::destroy_at(&m_data.i);break;
  case(kinds::identifier  ): std::destroy_at(&m_data.s);break;
  case(kinds::expression  ): std::destroy_at(&m_data.expr);break;
    }


  m_kind = kinds::null;

  return *this;
}


simple_value
operand::
evaluate() const noexcept
{
    switch(m_kind)
    {
  case(kinds::null        ): break;
  case(kinds::null_pointer): break;
  case(kinds::boolean     ): return m_data.b;break;
  case(kinds::string      ): break;
  case(kinds::integer     ): return m_data.i;break;
  case(kinds::fpn         ): return m_data.f;break;
  case(kinds::identifier  ): printf("%s",m_data.s.data());break;
  case(kinds::expression  ): return m_data.expr.evaluate();break;
    }


  return simple_value();
}


void
operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kinds::null        ): break;
  case(kinds::null_pointer): printf("nullptr");break;
  case(kinds::boolean     ): printf("%s",m_data.b? "true":"false");break;
  case(kinds::string      ): printf("\"%s\"",m_data.s.data());break;
  case(kinds::integer     ): printf("%" PRIi64,m_data.i);break;
  case(kinds::fpn         ): printf("%f",m_data.f);break;
  case(kinds::identifier  ): printf("%s",m_data.s.data());break;
  case(kinds::expression  ): m_data.expr.print();break;
    }
}




}




