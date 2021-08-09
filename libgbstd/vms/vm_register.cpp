#include"libgbstd/vm.hpp"
#include<cinttypes>




namespace gbstd{




vm_register&
vm_register::
assign(const vm_register&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind      = rhs.m_kind;
      m_type_info = rhs.m_type_info;
      m_label     = rhs.m_label;

        switch(m_kind)
        {
      case(kind::call     ): new(&m_data) vm_call(rhs.m_data.cal);break;
      case(kind::operation): new(&m_data) vm_operation(rhs.m_data.op);break;
      case(kind::phi      ): new(&m_data) vm_phi_element_list(rhs.m_data.phels);break;
        }
    }


  return *this;
}


vm_register&
vm_register::
assign(vm_register&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind     ,rhs.m_kind);
      std::swap(m_type_info,rhs.m_type_info);
      std::swap(m_label    ,rhs.m_label);

        switch(m_kind)
        {
      case(kind::call     ): new(&m_data) vm_call(std::move(rhs.m_data.cal));break;
      case(kind::operation): new(&m_data) vm_operation(std::move(rhs.m_data.op));break;
      case(kind::phi      ): new(&m_data) vm_phi_element_list(std::move(rhs.m_data.phels));break;
        }
    }


  return *this;
}


vm_register&
vm_register::
assign(vm_type_info  ti, std::u16string_view  lb, vm_call&&  ret) noexcept
{
  clear();

  m_kind = kind::call;

  m_type_info = ti;

  m_label = lb;

  new(&m_data) vm_call(std::move(ret));

  return *this;
}


vm_register&
vm_register::
assign(vm_type_info  ti, std::u16string_view  lb, vm_operation&&  op) noexcept
{
  clear();

  m_kind = kind::operation;

  m_type_info = ti;

  m_label = lb;

  new(&m_data) vm_operation(std::move(op));

  return *this;
}


vm_register&
vm_register::
assign(vm_type_info  ti, std::u16string_view  lb, vm_phi_element_list&&  phels) noexcept
{
  clear();

  m_kind = kind::phi;

  m_type_info = ti;

  m_label = lb;

  new(&m_data) vm_phi_element_list(std::move(phels));

  return *this;
}




void
vm_register::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::call): std::destroy_at(&m_data.cal);break;
  case(kind::operation): std::destroy_at(&m_data.op);break;
  case(kind::phi): std::destroy_at(&m_data.phels);break;
    }


  m_kind = kind::null;
}


void
vm_register::
print() const noexcept
{
  printf("reg ");

  m_type_info.print();

  printf(" ");

  gbstd::print(m_label);

  printf(" ");

    switch(m_kind)
    {
  case(kind::call): m_data.cal.print();break;
  case(kind::operation): m_data.op.print();break;
  case(kind::phi):
      printf("phi{");

        for(auto&  el: m_data.phels)
        {
          el.print();

          printf(",");
        }


      break;
    }
}




}




