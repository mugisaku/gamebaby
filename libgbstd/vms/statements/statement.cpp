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
      case(kind::for_      ): create_at(&m_data.fo ,std::move(rhs.m_data.fo));break;
      case(kind::while_    ): create_at(&m_data.whi,std::move(rhs.m_data.whi));break;
      case(kind::switch_   ): create_at(&m_data.swi,std::move(rhs.m_data.swi));break;
      case(kind::case_     ): create_at(&m_data.cas,std::move(rhs.m_data.cas));break;
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
assign(for_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.fo,std::move(st));

  m_kind = kind::for_;


  return *this;
}


statement&
statement::
assign(while_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.whi,std::move(st));

  m_kind = kind::while_;


  return *this;
}


statement&
statement::
assign(switch_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.swi,std::move(st));

  m_kind = kind::switch_;


  return *this;
}


statement&
statement::
assign(case_statement&&  st) noexcept
{
  clear();

  create_at(&m_data.cas,std::move(st));

  m_kind = kind::case_;


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
  case(kind::for_      ): std::destroy_at(&m_data.fo);break;
  case(kind::while_    ): std::destroy_at(&m_data.whi);break;
  case(kind::switch_   ): std::destroy_at(&m_data.swi);break;
  case(kind::case_     ): std::destroy_at(&m_data.cas);break;
  case(kind::control   ): std::destroy_at(&m_data.ctrl);break;
  case(kind::let       ): std::destroy_at(&m_data.let);break;
  case(kind::expression): std::destroy_at(&m_data.expr);break;
    }


  m_kind = kind::null;
}




void
statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
    switch(m_kind)
    {
  case(kind::return_   ): m_data.ret.compile(nd,ctx);break;
  case(kind::label     ): m_data.lb.compile(ctx);break;
  case(kind::jump      ): m_data.jmp.compile(ctx);break;
  case(kind::if_string ): m_data.ifs.compile(nd,ctx);break;
  case(kind::control   ): m_data.ctrl.compile(ctx);break;
  case(kind::let       ): m_data.let.compile(nd,ctx);break;
  case(kind::expression): m_data.expr.compile(nd,ctx);break;
  case(kind::for_      ): m_data.fo.compile(nd,ctx);break;
  case(kind::while_    ): m_data.whi.compile(nd,ctx);break;
  case(kind::switch_   ): m_data.swi.compile(nd,ctx);break;
  case(kind::case_     ): m_data.cas.compile(nd,ctx);break;
  case(kind::block     ): m_data.blk.compile(ctx);break;
    }
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
  case(kind::for_      ): m_data.fo.print();break;
  case(kind::while_    ): m_data.whi.print();break;
  case(kind::switch_   ): m_data.swi.print();break;
  case(kind::case_     ): m_data.cas.print();break;
  case(kind::block     ):
      printf("{\n");
      get_block().print();
      printf("}\n");
      break;
    }
}




}




