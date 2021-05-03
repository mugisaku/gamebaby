#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_statement::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::return_  ): std::destroy_at(&m_data.ret);break;
  case(kind::branch   ): std::destroy_at(&m_data.br);break;
  case(kind::label    ): std::destroy_at(&m_data.lb);break;
  case(kind::store    ): std::destroy_at(&m_data.st);break;
  case(kind::register_): std::destroy_at(&m_data.reg);break;
    }


  m_kind = kind::null;
}


ir_statement&
ir_statement::
assign(const ir_statement&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::return_  ): new(&m_data) ir_return_statement(rhs.m_data.ret);break;
      case(kind::branch   ): new(&m_data) ir_branch_statement(rhs.m_data.br);break;
      case(kind::label    ): new(&m_data) ir_label_statement(rhs.m_data.lb);break;
      case(kind::store    ): new(&m_data) ir_store_statement(rhs.m_data.st);break;
      case(kind::register_): new(&m_data) ir_register_statement(rhs.m_data.reg);break;
        }
    }


  return *this;
}


ir_statement&
ir_statement::
assign(ir_statement&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::return_  ): new(&m_data) ir_return_statement(std::move(rhs.m_data.ret));break;
      case(kind::branch   ): new(&m_data) ir_branch_statement(std::move(rhs.m_data.br));break;
      case(kind::label    ): new(&m_data) ir_label_statement(std::move(rhs.m_data.lb));break;
      case(kind::store    ): new(&m_data) ir_store_statement(std::move(rhs.m_data.st));break;
      case(kind::register_): new(&m_data) ir_register_statement(std::move(rhs.m_data.reg));break;
        }
    }


  return *this;
}


ir_statement&
ir_statement::
assign(ir_return_statement&&  ret) noexcept
{
  clear();

  m_kind = kind::return_;

  new(&m_data) ir_return_statement(std::move(ret));

  return *this;
}


ir_statement&
ir_statement::
assign(ir_branch_statement&&  br) noexcept
{
  clear();

  m_kind = kind::branch;

  new(&m_data) ir_branch_statement(std::move(br));

  return *this;
}


ir_statement&
ir_statement::
assign(ir_label_statement&&  lb) noexcept
{
  clear();

  m_kind = kind::label;

  new(&m_data) ir_label_statement(std::move(lb));

  return *this;
}


ir_statement&
ir_statement::
assign(ir_store_statement&&  st) noexcept
{
  clear();

  m_kind = kind::store;

  new(&m_data) ir_store_statement(std::move(st));

  return *this;
}


ir_statement&
ir_statement::
assign(ir_register_statement&&  reg) noexcept
{
  clear();

  m_kind = kind::register_;

  new(&m_data) ir_register_statement(std::move(reg));

  return *this;
}


void
ir_statement::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::return_):
      printf("<ret> ");

      gbstd::print(m_data.ret.operand());
      break;
  case(kind::branch):
      printf("<br> ");

      gbstd::print(m_data.br.operand());

      printf(" ");

      gbstd::print(m_data.br.label());
      break;
  case(kind::label):
      printf("<lb> ");

      gbstd::print(m_data.lb.identifier());
      break;
  case(kind::store):
      printf("<st> ");

      m_data.st.type_info().print();

      printf(" ");

      gbstd::print(m_data.st.address_operand());

      printf(" ");

      gbstd::print(m_data.st.value_operand());
      break;
  case(kind::register_):
      printf("<reg> ");

      gbstd::print(m_data.reg.identifier());

      printf(" ");

      m_data.reg.operation().print();
      break;
    }
}




}




