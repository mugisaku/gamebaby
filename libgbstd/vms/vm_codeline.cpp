#include"libgbstd/vm.hpp"




namespace gbstd{




codeline&
codeline::
assign(const codeline&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind     = rhs.m_kind;
      m_function = rhs.m_function;

        switch(m_kind)
        {
      case(kind::store_instruction ): new(&m_data) store_instruction(rhs.m_data.st);break;
      case(kind::branch_instruction): new(&m_data) branch_instruction(rhs.m_data.br);break;
      case(kind::return_instruction): new(&m_data) return_instruction(rhs.m_data.ret);break;

      case(kind::operation): new(&m_data) operation(rhs.m_data.op);break;
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

      std::swap(m_kind    ,rhs.m_kind    );
      std::swap(m_function,rhs.m_function);

        switch(m_kind)
        {
      case(kind::store_instruction ): new(&m_data) store_instruction(std::move(rhs.m_data.st));break;
      case(kind::branch_instruction): new(&m_data) branch_instruction(std::move(rhs.m_data.br));break;
      case(kind::return_instruction): new(&m_data) return_instruction(std::move(rhs.m_data.ret));break;

      case(kind::operation): new(&m_data) operation(std::move(rhs.m_data.op));break;
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

  case(kind::operation ): m_data.op.~operation();break;
    }


  m_kind = kind::null;

  m_function = nullptr;
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
set_content(operation&&  op) noexcept
{
  clear();

  new(&m_data) operation(std::move(op));

  m_kind = kind::operation;

  return *this;
}




void
codeline::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::store_instruction ): m_data.st.print();break;
  case(kind::branch_instruction): m_data.br.print();break;
  case(kind::return_instruction): m_data.ret.print();break;

  case(kind::operation ): m_data.op.print();break;
    }
}




}




