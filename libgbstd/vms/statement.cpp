#include"libgbstd/vm.hpp"




namespace gbstd{




/*
statement&
statement::
assign(const statement&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind  = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::return_   ): new(&m_data) return_statement(rhs.m_data.ret);break;
      case(kind::label     ): new(&m_data) label_statement(rhs.m_data.lb);break;
      case(kind::jump      ): new(&m_data) jump_statement(rhs.m_data.jmp);break;
      case(kind::if_string ): new(&m_data) if_string_statement(rhs.m_data.ifs);break;
      case(kind::block     ): new(&m_data) block_statement(rhs.m_data.blk);break;
      case(kind::control   ): new(&m_data) control_statement(rhs.m_data.ctrl);break;
      case(kind::let       ): new(&m_data) let_statement(rhs.m_data.let);break;
      case(kind::expression): new(&m_data) expression(rhs.m_data.expr);break;
        }
    }


  return *this;
}
*/


statement&
statement::
assign(statement&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::return_   ): create_at(&m_data.ret,std::move(rhs.m_data.ret));break;
      case(kind::label     ): create_at(&m_data.lb,std::move(rhs.m_data.lb));break;
      case(kind::jump      ): create_at(&m_data.jmp,std::move(rhs.m_data.jmp));break;
      case(kind::if_string ): create_at(&m_data.ifs,std::move(rhs.m_data.ifs));break;
      case(kind::block     ): create_at(&m_data.blk,std::move(rhs.m_data.blk));break;
      case(kind::control   ): create_at(&m_data.ctrl,std::move(rhs.m_data.ctrl));break;
      case(kind::let       ): create_at(&m_data.let,std::move(rhs.m_data.let));break;
      case(kind::expression): create_at(&m_data.expr,std::move(rhs.m_data.expr));break;
        }
    }


  return *this;
}




statement&
statement::
assign(return_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.ret,std::move(st));

  m_kind = kind::return_;


  return *this;
}


statement&
statement::
assign(label_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.lb,std::move(st));

  m_kind = kind::label;


  return *this;
}


statement&
statement::
assign(jump_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.jmp,std::move(st));

  m_kind = kind::jump;


  return *this;
}


statement&
statement::
assign(if_string_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.ifs,std::move(st));

  m_kind = kind::if_string;


  return *this;
}


statement&
statement::
assign(block_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.blk,std::move(st));

  m_kind = kind::block;


  return *this;
}


statement&
statement::
assign(control_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.ctrl,std::move(st));

  m_kind = kind::control;


  return *this;
}


statement&
statement::
assign(let_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.let,std::move(st));

  m_kind = kind::let;


  return *this;
}


statement&
statement::
assign(expression_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.expr,std::move(st));

  m_kind = kind::expression;


  return *this;
}




void
statement::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::return_   ): std::destroy_at(&m_data.ret);break;
  case(kind::label     ): std::destroy_at(&m_data.lb);break;
  case(kind::jump      ): std::destroy_at(&m_data.jmp);break;
  case(kind::if_string ): std::destroy_at(&m_data.ifs);break;
  case(kind::block     ): std::destroy_at(&m_data.blk);break;
  case(kind::control   ): std::destroy_at(&m_data.ctrl);break;
  case(kind::let       ): std::destroy_at(&m_data.let);break;
  case(kind::expression): std::destroy_at(&m_data.expr);break;
    }


  m_kind = kind::null;
}




void
statement::
print(const context*  ctx, const function*  fn) const noexcept
{
    switch(m_kind)
    {
  case(kind::return_   ): get_return().print();break;
  case(kind::label     ): get_label().print();break;
  case(kind::jump      ): get_jump().print();break;
  case(kind::if_string ): get_if_string().print();break;
  case(kind::control   ): get_control().print();break;
  case(kind::let       ): get_let().print();break;
  case(kind::expression): get_expression().print();break;
  case(kind::block     ):
      printf("{\n");
      get_block().print();
      printf("}\n");
      break;
    }
}




value
expression_statement::
evaluate(const context&  ctx) const noexcept
{
  auto&  ls = m_expression.get_elements();

  auto    p = ls.data();
  auto  end = ls.data()+ls.size();

    
  return value();
}




}




