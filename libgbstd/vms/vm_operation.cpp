#include"libgbstd/vm.hpp"




namespace gbstd{




/*
operation&
operation::
assign(const operation&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind     = rhs.m_kind;
      m_variable = rhs.m_variable;

        switch(m_kind)
        {
      case(kind::unary ): new(&m_data) unary_operation(rhs.m_data.un);break;
      case(kind::binary): new(&m_data) binary_operation(rhs.m_data.bin);break;
      case(kind::call  ): new(&m_data) call_operation(rhs.m_data.cal);break;
        }
    }


  return *this;
}
*/


operation&
operation::
assign(operation&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unset_content();

      std::swap(m_kind     ,rhs.m_kind     );
      std::swap(m_type_info,rhs.m_type_info);
      std::swap(m_label    ,rhs.m_label    );

        switch(m_kind)
        {
      case(kind::unary ): new(&m_data) unary_operation(std::move(rhs.m_data.un));break;
      case(kind::binary): new(&m_data) binary_operation(std::move(rhs.m_data.bin));break;
      case(kind::call  ): new(&m_data) call_operation(std::move(rhs.m_data.cal));break;
        }
    }


  return *this;
}


operation&
operation::
assign(unary_operation&&  un) noexcept
{
  return set_content(std::move(un));
}


operation&
operation::
assign(binary_operation&&  bin) noexcept
{
  return set_content(std::move(bin));
}


operation&
operation::
assign(call_operation&&  cal) noexcept
{
  return set_content(std::move(cal));
}




void
operation::
unset_content() noexcept
{
    switch(m_kind)
    {
  case(kind::unary ): m_data.un.~unary_operation();break;
  case(kind::binary): m_data.bin.~binary_operation();break;
  case(kind::call  ): m_data.cal.~call_operation();break;
    }


  m_kind = kind::null;
}


operation&
operation::
set_content(unary_operation&&  un) noexcept
{
  unset_content();

  new(&m_data) unary_operation(std::move(un));

  m_kind = kind::unary;

  return *this;
}


operation&
operation::
set_content(binary_operation&&  bin) noexcept
{
  unset_content();

  new(&m_data) binary_operation(std::move(bin));

  m_kind = kind::binary;

  return *this;
}


operation&
operation::
set_content(call_operation&&  cal) noexcept
{
  unset_content();

  new(&m_data) call_operation(std::move(cal));

  m_kind = kind::call;

  return *this;
}


value
operation::
operator()(execution_frame&  frm) const noexcept
{
    switch(m_kind)
    {
  case(kind::unary ): return m_data.un( frm);break;
  case(kind::binary): return m_data.bin(frm);break;
  case(kind::call  ): return m_data.cal(frm);break;
    }


  return value();
}


void
operation::
print() const noexcept
{
  m_type_info->print();

  printf("  %%%s = ",m_label.data());

    switch(m_kind)
    {
  case(kind::unary ): m_data.un.print(); break;
  case(kind::binary): m_data.bin.print();break;
  case(kind::call  ): m_data.cal.print();break;
    }
}




}




