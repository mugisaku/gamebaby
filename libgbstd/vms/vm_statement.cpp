#include"libgbstd/vm.hpp"




namespace gbstd{




namespace statements{
const statement  null;
}


statement&
statement::
assign(statement&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind   ,rhs.m_kind   );
      std::swap(m_pointer,rhs.m_pointer);
    }


  return *this;
}


statement&
statement::
assign(return_statement*  st) noexcept
{
  clear();

  m_pointer = st;

  m_kind = kind::return_;


  return *this;
}


statement&
statement::
assign(label_statement*  st) noexcept
{
  clear();

  m_pointer = st;

  m_kind = kind::label;


  return *this;
}


statement&
statement::
assign(jump_statement*  st) noexcept
{
  clear();

  m_pointer = st;

  m_kind = kind::jump;


  return *this;
}


statement&
statement::
assign(if_string_statement*  st) noexcept
{
  clear();

  m_pointer = st;

  m_kind = kind::if_string;


  return *this;
}


statement&
statement::
assign(block_statement*  st) noexcept
{
  clear();

  m_pointer = st;

  m_kind = kind::block;


  return *this;
}


statement&
statement::
assign(control_statement*  st) noexcept
{
  clear();

  m_pointer = st;

  m_kind = kind::control;


  return *this;
}


statement&
statement::
assign(expression_statement*  st) noexcept
{
  clear();

  m_pointer = st;

  m_kind = kind::expression;


  return *this;
}




void
statement::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::return_   ): delete static_cast<return_statement*>(m_pointer);break;
  case(kind::label     ): delete static_cast<label_statement*>(m_pointer);break;
  case(kind::jump      ): delete static_cast<jump_statement*>(m_pointer);break;
  case(kind::if_string ): delete static_cast<if_string_statement*>(m_pointer);break;
  case(kind::block     ): delete static_cast<block_statement*>(m_pointer);break;
  case(kind::control   ): delete static_cast<control_statement*>(m_pointer);break;
  case(kind::expression): delete static_cast<expression_statement*>(m_pointer);break;
    }


  m_kind    = kind::null;
  m_pointer =    nullptr;
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
  case(kind::block     ): get_block().print();break;
  case(kind::control   ): get_control().print();break;
  case(kind::expression): get_expression().print();break;
    }
}




}




