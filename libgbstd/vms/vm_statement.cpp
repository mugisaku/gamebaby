#include"libgbstd/vm.hpp"




namespace gbstd{




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
      case(kind::return_   ): new(&m_data) return_statement(std::move(rhs.m_data.ret));break;
      case(kind::label     ): new(&m_data) label_statement(std::move(rhs.m_data.lb));break;
      case(kind::jump      ): new(&m_data) jump_statement(std::move(rhs.m_data.jmp));break;
      case(kind::if_string ): new(&m_data) if_string_statement(std::move(rhs.m_data.ifs));break;
      case(kind::block     ): new(&m_data) block_statement(std::move(rhs.m_data.blk));break;
      case(kind::expression): new(&m_data) expression_statement(std::move(rhs.m_data.expr));break;
        }
      }


  return *this;
}


statement&
statement::
assign(return_statement&&  st) noexcept
{
  clear();

  new(&m_data) return_statement(std::move(st));

  m_kind = kind::return_;


  return *this;
}


statement&
statement::
assign(label_statement&&  st) noexcept
{
  clear();

  new(&m_data) label_statement(std::move(st));

  m_kind = kind::label;


  return *this;
}


statement&
statement::
assign(jump_statement&&  st) noexcept
{
  clear();

  new(&m_data) jump_statement(std::move(st));

  m_kind = kind::jump;


  return *this;
}


statement&
statement::
assign(if_string_statement&&  st) noexcept
{
  clear();

  new(&m_data) if_string_statement(std::move(st));

  m_kind = kind::if_string;


  return *this;
}


statement&
statement::
assign(block_statement&&  st) noexcept
{
  clear();

  new(&m_data) block_statement(std::move(st));

  m_kind = kind::block;


  return *this;
}


statement&
statement::
assign(expression_statement&&  st) noexcept
{
  clear();

  new(&m_data) expression_statement(std::move(st));

  m_kind = kind::expression;


  return *this;
}




void
statement::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::return_   ): m_data.ret.~return_statement();break;
  case(kind::label     ): m_data.lb.~label_statement();break;
  case(kind::jump      ): m_data.jmp.~jump_statement();break;
  case(kind::if_string ): m_data.ifs.~if_string_statement();break;
  case(kind::block     ): m_data.blk.~block_statement();break;
  case(kind::expression): m_data.expr.~expression_statement();break;
    }


  m_kind = kind::null;
}


void
statement::
print(const context*  ctx, const function*  fn) const noexcept
{
    switch(m_kind)
    {
  case(kind::return_   ): m_data.ret.print();break;
  case(kind::label     ): m_data.lb.print();break;
  case(kind::jump      ): m_data.jmp.print();break;
  case(kind::if_string ): m_data.ifs.print();break;
  case(kind::block     ): m_data.blk.print();break;
  case(kind::expression): m_data.expr.print();break;
    }
}




}




