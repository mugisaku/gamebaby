#include"libgbstd/vm.hpp"




namespace gbstd{




statement_cursor&
statement_cursor::
assign(const block_statement&  st) noexcept
{
  m_frame_stack.clear();

  m_frame_stack.emplace_back(st);

  return *this;
}


statement_cursor&
statement_cursor::
push(const block_statement&  st) noexcept
{
  m_frame_stack.clear();

  m_frame_stack.emplace_back(st);

  advance();

  return *this;
}


const block_statement&
statement_cursor::
pop() noexcept
{
  auto&  ref = m_frame_stack.back().m_block;

  m_frame_stack.pop_back();

  return ref;
}


void
statement_cursor::
advance() noexcept
{
  m_frame_stack.back().m_counter += 1;
}


const statement&
statement_cursor::
get() const noexcept
{
  auto&  frm = m_frame_stack.back();

  return frm.m_block[frm.m_counter];
}


}




