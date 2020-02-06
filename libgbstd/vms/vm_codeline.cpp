#include"libgbstd/vm.hpp"




namespace gbstd{




/*
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
*/


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
assign(store_instruction&&  st) noexcept
{
  clear();

  new(&m_data) store_instruction(std::move(st));

  m_kind = kind::store_instruction;

  return *this;
}


codeline&
codeline::
assign(branch_instruction&&  br) noexcept
{
  clear();

  new(&m_data) branch_instruction(std::move(br));

  m_kind = kind::branch_instruction;

  return *this;
}


codeline&
codeline::
assign(return_instruction&&  ret) noexcept
{
  clear();

  new(&m_data) return_instruction(std::move(ret));

  m_kind = kind::return_instruction;

  return *this;
}


codeline&
codeline::
assign(operation&&  op) noexcept
{
  clear();

  new(&m_data) operation(std::move(op));

  m_kind = kind::operation;

  return *this;
}




void
codeline::
print(const context*  ctx, const function*  fn) const noexcept
{
report;
    switch(m_kind)
    {
  case(kind::store_instruction ):
       printf("st ");
       m_data.st.get_destination().print(ctx,fn);
       printf(" ");
       m_data.st.get_source().print(ctx,fn);
       break;
  case(kind::branch_instruction):
       printf("br ");
       m_data.br.get_condition().print(ctx,fn);
       printf(" ");
       m_data.br.get_destination().print(ctx,fn);
       break;
  case(kind::return_instruction):
       printf("ret ");
       m_data.ret.get_operand().print(ctx,fn);
       break;
  case(kind::operation ):
       m_data.op.print(ctx,fn);
       break;
    }
}




}




