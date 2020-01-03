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
assign(std::string_view  lb, unary_operation&&  un) noexcept
{
  set_label(lb);

  return set_content(std::move(un));
}


operation&
operation::
assign(std::string_view  lb, binary_operation&&  bin) noexcept
{
  set_label(lb);

  return set_content(std::move(bin));
}


operation&
operation::
assign(std::string_view  lb, call_operation&&  cal) noexcept
{
  set_label(lb);

  return set_content(std::move(cal));
}




void
operation::
clear() noexcept
{
  m_label.clear();

  unset_content();
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
evaluate(execution_frame&  frm) const noexcept
{
  value  v;

    switch(m_kind)
    {
  case(kind::unary ): v = m_data.un.evaluate(frm); break;
  case(kind::binary): v = m_data.bin.evaluate(frm);break;
  case(kind::call  ): v = m_data.cal.evaluate(frm);break;
    }


  auto&  var = frm.push_variable(m_label,std::move(v));

  return var.get_value();
}


void
operation::
print() const noexcept
{
  printf("%%%s = ",m_label.data());

    switch(m_kind)
    {
  case(kind::unary ): m_data.un.print(); break;
  case(kind::binary): m_data.bin.print();break;
  case(kind::call  ): m_data.cal.print();break;
    }
}




}




