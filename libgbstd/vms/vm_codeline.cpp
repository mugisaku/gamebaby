#include"libgbstd/vm.hpp"




namespace gbstd{




codeline&
codeline::
assign(const codeline&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind  = rhs.m_kind;
      m_block = rhs.m_block;
      m_label = rhs.m_label;

        switch(m_kind)
        {
      case(kind::store_instruction ): new(&m_data) store_instruction(rhs.m_data.st);break;
      case(kind::branch_instruction): new(&m_data) branch_instruction(rhs.m_data.br);break;
      case(kind::return_instruction): new(&m_data) return_instruction(rhs.m_data.ret);break;

      case(kind::unary_operation ): new(&m_data) unary_operation(rhs.m_data.unop);break;
      case(kind::binary_operation): new(&m_data) binary_operation(rhs.m_data.binop);break;
      case(kind::phi_operation   ): new(&m_data) phi_operation(rhs.m_data.phiop);break;
      case(kind::call_operation  ): new(&m_data) call_operation(rhs.m_data.calop);break;
        }
    }


  return *this;
}


codeline&
codeline::
assign(codeline&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind ,rhs.m_kind );
      std::swap(m_block,rhs.m_block);
      std::swap(m_label,rhs.m_label);

        switch(m_kind)
        {
      case(kind::store_instruction ): new(&m_data) store_instruction(std::move(rhs.m_data.st));break;
      case(kind::branch_instruction): new(&m_data) branch_instruction(std::move(rhs.m_data.br));break;
      case(kind::return_instruction): new(&m_data) return_instruction(std::move(rhs.m_data.ret));break;

      case(kind::unary_operation ): new(&m_data) unary_operation(std::move(rhs.m_data.unop));break;
      case(kind::binary_operation): new(&m_data) binary_operation(std::move(rhs.m_data.binop));break;
      case(kind::phi_operation   ): new(&m_data) phi_operation(std::move(rhs.m_data.phiop));break;
      case(kind::call_operation  ): new(&m_data) call_operation(std::move(rhs.m_data.calop));break;
        }
    }


  return *this;
}


void
codeline::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::store_instruction ): m_data.st.~store_instruction();break;
  case(kind::branch_instruction): m_data.br.~branch_instruction();break;
  case(kind::return_instruction): m_data.ret.~return_instruction();break;

  case(kind::unary_operation ): m_data.unop.~unary_operation();break;
  case(kind::binary_operation): m_data.binop.~binary_operation();break;
  case(kind::phi_operation   ): m_data.phiop.~phi_operation();break;
  case(kind::call_operation  ): m_data.calop.~call_operation();break;
    }


  m_kind = kind::null;

  m_block = nullptr;

  m_label.clear();
}


codeline&
codeline::
set_content(store_instruction&&  st) noexcept
{
  clear();

  new(&m_data) store_instruction(std::move(st));

  m_kind = kind::store_instruction;

  return *this;
}


codeline&
codeline::
set_content(branch_instruction&&  br) noexcept
{
  clear();

  new(&m_data) branch_instruction(std::move(br));

  m_kind = kind::branch_instruction;

  return *this;
}


codeline&
codeline::
set_content(return_instruction&&  ret) noexcept
{
  clear();

  new(&m_data) return_instruction(std::move(ret));

  m_kind = kind::return_instruction;

  return *this;
}


codeline&
codeline::
set_content(unary_operation&&  unop) noexcept
{
  clear();

  new(&m_data) unary_operation(std::move(unop));

  m_kind = kind::unary_operation;

  return *this;
}


codeline&
codeline::
set_content(binary_operation&&  binop) noexcept
{
  clear();

  new(&m_data) binary_operation(std::move(binop));

  m_kind = kind::binary_operation;

  return *this;
}


codeline&
codeline::
set_content(phi_operation&&  phiop) noexcept
{
  clear();

  new(&m_data) phi_operation(std::move(phiop));

  m_kind = kind::phi_operation;

  return *this;
}


codeline&
codeline::
set_content(call_operation&&  calop) noexcept
{
  clear();

  new(&m_data) call_operation(std::move(calop));

  m_kind = kind::call_operation;

  return *this;
}



}




