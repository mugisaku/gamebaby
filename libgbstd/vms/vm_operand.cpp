#include"libgbstd/vm.hpp"




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
      case(kind::identifier     ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::integer_literal): new(&m_data) int64_t(rhs.m_data.i);break;
      case(kind::pointer_literal): new(&m_data) multi_pointer(rhs.m_data.p);break;
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

      std::swap(m_kind ,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::identifier     ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::integer_literal): new(&m_data) int64_t(std::move(rhs.m_data.i));break;
      case(kind::pointer_literal): new(&m_data) multi_pointer(std::move(rhs.m_data.p));break;
        }
    }


  return *this;
}


operand&
operand::
assign(std::string_view  sv) noexcept
{
  clear();

  new(&m_data) std::string(sv);

  m_kind = kind::identifier;

  return *this;
}


operand&
operand::
assign(int64_t  i) noexcept
{
  clear();

  new(&m_data) int64_t(i);

  m_kind = kind::integer_literal;

  return *this;
}


operand&
operand::
assign(multi_pointer  p) noexcept
{
  clear();

  new(&m_data) multi_pointer(p);

  m_kind = kind::pointer_literal;

  return *this;
}


void
operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::identifier     ): m_data.s.~basic_string();break;
  case(kind::integer_literal): /*m_data.i.~int()*/;break;
  case(kind::pointer_literal): m_data.p.~multi_pointer();break;
    }


  m_kind = kind::null;
}


value
operand::
evaluate(context&  ctx) const noexcept
{
  auto&  tc = ctx.get_type_collection();

    if(is_integer_literal())
    {
      return ctx.make_value(m_data.i);
    }

  else
    if(is_identifier())
    {
      printf("%s is not resolved",m_data.s.data());
    }

  else
    if(is_pointer_literal())
    {
      auto  p = get_pointer();

        if(p.is_null())
        {
          return ctx.make_value(nullptr);
        }

      else
        if(p.is_global())
        {
          auto&  var = ctx.get_global_variable(p.get());

          auto&  t = var.get_value().get_type_derivation().get_reference_type();

          return value(t,p.get_packed());
        }

      else
        if(p.is_local())
        {
          auto&  var = ctx.get_local_variable(p.get());

          auto&  t = var.get_value().get_type_derivation().get_reference_type();

          return value(t,p.get_packed());
        }

      else
        if(p.is_function())
        {
          auto&  fn = ctx.get_function(p.get());

          return value(fn.get_signature(),static_cast<uint64_t>(p.get()));
        }
    }


  return value();
}


void
operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::identifier     ): printf("%s",m_data.s.data());break;
  case(kind::integer_literal): printf("%" PRIi64,m_data.i);break;
  case(kind::pointer_literal): printf("%" PRIu32,m_data.p.get());break;
    }
}




}




