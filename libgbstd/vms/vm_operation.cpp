#include"libgbstd/vm.hpp"




namespace gbstd{




operation&
operation::
assign(const operation&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind  = rhs.m_kind;
      m_label = rhs.m_label;

      *m_result = *rhs.m_result;

        switch(m_kind)
        {
      case(kind::unary ): new(&m_data) unary_operation(rhs.m_data.un);break;
      case(kind::binary): new(&m_data) binary_operation(rhs.m_data.bin);break;
      case(kind::call  ): new(&m_data) call_operation(rhs.m_data.cal);break;
        }
    }


  return *this;
}


operation&
operation::
assign(operation&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind ,rhs.m_kind );
      std::swap(m_label,rhs.m_label);
      std::swap(m_result,rhs.m_result);

        switch(m_kind)
        {
      case(kind::unary ): new(&m_data) unary_operation(std::move(rhs.m_data.un));break;
      case(kind::binary): new(&m_data) binary_operation(std::move(rhs.m_data.bin));break;
      case(kind::call  ): new(&m_data) call_operation(std::move(rhs.m_data.cal));break;
        }
    }


  return *this;
}


void
operation::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::unary ): m_data.un.~unary_operation();break;
  case(kind::binary): m_data.bin.~binary_operation();break;
  case(kind::call  ): m_data.cal.~call_operation();break;
    }


  m_kind = kind::null;

  m_label.clear();

  *m_result = result();
}


operation&
operation::
set_content(unary_operation&&  un) noexcept
{
  clear();

  new(&m_data) unary_operation(std::move(un));

  m_kind = kind::unary;

  return *this;
}


operation&
operation::
set_content(binary_operation&&  bin) noexcept
{
  clear();

  new(&m_data) binary_operation(std::move(bin));

  m_kind = kind::binary;

  return *this;
}


operation&
operation::
set_content(call_operation&&  cal) noexcept
{
  clear();

  new(&m_data) call_operation(std::move(cal));

  m_kind = kind::call;

  return *this;
}


bool
operation::
is_constant(const function&  fn) const noexcept
{
    if(m_result->m_status.test(result::test_flag))
    {
      return m_result->m_status.test(result::constant_flag);
    }


  bool  flag = false;

    switch(m_kind)
    {
  case(kind::unary ): flag = m_data.un.is_constant( fn);break;
  case(kind::binary): flag = m_data.bin.is_constant(fn);break;
    }


  m_result->m_status.set(result::test_flag);

    if(flag)
    {
      m_result->m_status.set(result::constant_flag);
    }


  return flag;
}


const value&
operation::
evaluate(execution_frame&  frm) const noexcept
{
    if(!m_result->m_value.is_undefined() && is_constant(frm.get_function()))
    {
      return m_result->m_value;
    }


  auto&  v = m_result->m_value;

    switch(m_kind)
    {
  case(kind::unary ): v = m_data.un.evaluate(frm); break;
  case(kind::binary): v = m_data.bin.evaluate(frm);break;
  case(kind::call  ): v = m_data.cal.evaluate(frm);break;
    }


  return m_result->m_value;
}


void
operation::
print() const noexcept
{
}




}




