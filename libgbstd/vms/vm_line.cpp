#include"libgbstd/vm.hpp"
#include<cinttypes>




namespace gbstd{




vm_line&
vm_line::
assign(const vm_line&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind     = rhs.m_kind;
      m_position = rhs.m_position;

        switch(m_kind)
        {
      case(kind::ret): new(&m_data) vm_return(rhs.m_data.ret);break;
      case(kind::br ): new(&m_data) vm_branch(rhs.m_data.br );break;
      case(kind::jmp): new(&m_data) vm_jump(rhs.m_data.jmp);break;
      case(kind::trf): new(&m_data) vm_transfer(rhs.m_data.trf);break;
      case(kind::cal): new(&m_data) vm_call(rhs.m_data.cal);break;
      case(kind::reg): new(&m_data) vm_register(rhs.m_data.reg);break;
        }
    }


  return *this;
}


vm_line&
vm_line::
assign(vm_line&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind    ,rhs.m_kind);
      std::swap(m_position,rhs.m_position);

        switch(m_kind)
        {
      case(kind::ret): new(&m_data) vm_return(std::move(rhs.m_data.ret));break;
      case(kind::br ): new(&m_data) vm_branch(std::move(rhs.m_data.br ));break;
      case(kind::jmp): new(&m_data) vm_jump(std::move(rhs.m_data.jmp));break;
      case(kind::trf): new(&m_data) vm_transfer(std::move(rhs.m_data.trf));break;
      case(kind::cal): new(&m_data) vm_call(std::move(rhs.m_data.cal));break;
      case(kind::reg): new(&m_data) vm_register(std::move(rhs.m_data.reg));break;
        }
    }


  return *this;
}


vm_line&
vm_line::
assign(vm_return&&  ret) noexcept
{
  clear();

  m_kind = kind::ret;

  new(&m_data) vm_return(std::move(ret));

  return *this;
}


vm_line&
vm_line::
assign(vm_branch&&  br) noexcept
{
  clear();

  m_kind = kind::br;

  new(&m_data) vm_branch(std::move(br));

  return *this;
}


vm_line&
vm_line::
assign(vm_jump&&  jmp) noexcept
{
  clear();

  m_kind = kind::jmp;

  new(&m_data) vm_jump(std::move(jmp));

  return *this;
}


vm_line&
vm_line::
assign(vm_transfer&&  trf) noexcept
{
  clear();

  m_kind = kind::trf;

  new(&m_data) vm_transfer(std::move(trf));

  return *this;
}


vm_line&
vm_line::
assign(vm_call&&  cal) noexcept
{
  clear();

  m_kind = kind::cal;

  new(&m_data) vm_call(std::move(cal));

  return *this;
}


vm_line&
vm_line::
assign(vm_register&&  reg) noexcept
{
  clear();

  m_kind = kind::reg;

  new(&m_data) vm_register(std::move(reg));

  return *this;
}




void
vm_line::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::ret): std::destroy_at(&m_data.ret);break;
  case(kind::br ): std::destroy_at(&m_data.br );break;
  case(kind::jmp): std::destroy_at(&m_data.jmp);break;
  case(kind::trf): std::destroy_at(&m_data.trf);break;
  case(kind::cal): std::destroy_at(&m_data.cal);break;
  case(kind::reg): std::destroy_at(&m_data.reg);break;
    }


  m_kind = kind::null;

  m_position = 0;
}


void
vm_line::
print() const noexcept
{
  printf("%4d| ",m_position);

  printf(" ");

    switch(m_kind)
    {
  case(kind::ret): m_data.ret.print();break;
  case(kind::br ): m_data.br.print();break;
  case(kind::jmp): m_data.jmp.print();break;
  case(kind::trf): m_data.trf.print();break;
  case(kind::cal): m_data.cal.print();break;
  case(kind::reg): m_data.reg.print();break;
    }
}




}




