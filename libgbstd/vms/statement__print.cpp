#include"libgbstd/vm.hpp"




namespace gbstd{




void
expression_statement::
print() const noexcept
{
  m_expression.print();
}


void
block_statement::
print() const noexcept
{
  printf("{\n");

    for(auto&  st: m_space->get_statement_list())
    {
      st.print();

      printf("\n");
    }

  printf("}");
}


void
if_statement::
print() const noexcept
{
  printf("if(");

  m_expression.print();

  printf(")");

  block_statement::print();
}


void
if_string_statement::
print() const noexcept
{
}


void
for_statement::
print() const noexcept
{
  printf("for(");

  m_init.print();

  printf(";");

  m_cond.print();

  printf(";");

  m_loop.print();

  printf(")");

  block_statement::print();
}


void
while_statement::
print() const noexcept
{
  printf("while(");

  get_expression().print();

  printf(")");

  block_statement::print();
}


void
case_statement::
print() const noexcept
{
  printf("case ");

  get_expression().print();

  printf(":");
}


void
switch_statement::
print() const noexcept
{
  printf("switch(");

  get_expression().print();

  printf(")");

  block_statement::print();
}


void
return_statement::
print() const noexcept
{
  printf("return ");

  get_expression().print();
}


void
label_statement::
print() const noexcept
{
  printf("label:%s",m_string.data());
}


void
jump_statement::
print() const noexcept
{
  printf("jump %s",m_string.data());
}


void
control_statement::
print() const noexcept
{
  printf("%s",m_string.data());
}


void
let_statement::
print() const noexcept
{
  printf("let %s",m_target_name.data());

    if(m_expression)
    {
      printf(" = (");

      m_expression.print();

      printf(")");
    }
}




}




