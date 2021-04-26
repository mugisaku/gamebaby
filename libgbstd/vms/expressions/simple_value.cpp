#include"libgbstd/vms/expression.hpp"




namespace gbstd{





simple_value&
simple_value::
assign(const simple_value&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind   = rhs.m_kind;
      m_offset = rhs.m_offset;

        switch(m_kind)
        {
      case(kind::integer ): m_data.i = rhs.m_data.i;break;
      case(kind::boolean ): m_data.b = rhs.m_data.b;break;
      case(kind::floating): m_data.f = rhs.m_data.f;break;
      case(kind::object  ): m_data.o = clone(rhs.m_data.o);break;
        }
    }


  return *this;
}


simple_value&
simple_value::
assign(simple_value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind  ,rhs.m_kind);
      std::swap(m_offset,rhs.m_offset);

        switch(m_kind)
        {
      case(kind::integer ): m_data.i = rhs.m_data.i;break;
      case(kind::boolean ): m_data.b = rhs.m_data.b;break;
      case(kind::floating): m_data.f = rhs.m_data.f;break;
      case(kind::object  ): m_data.o = rhs.m_data.o;break;
        }
    }


  return *this;
}


simple_value&
simple_value::
assign(int64_t  i) noexcept
{
  clear();

  m_kind = kind::integer;

  m_data.i = i;

  return *this;
}


simple_value&
simple_value::
assign(bool  b) noexcept
{
  clear();

  m_kind = kind::boolean;

  m_data.b = b;

  return *this;
}


simple_value&
simple_value::
assign(double   f) noexcept
{
  clear();

  m_kind = kind::floating;

  m_data.f = f;

  return *this;
}


simple_value&
simple_value::
assign(simple_object&&  o) noexcept
{
  clear();

  m_kind = kind::object;

  m_data.o = new simple_object(std::move(o));

  return *this;
}


void
simple_value::
clear() noexcept
{
    if(is_object())
    {
      std::destroy_at(m_data.o);
    }


  m_kind = kind::null;

  m_offset = 0;
}




}




