#include"libgbstd/parser.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




const token
token_block_view::
m_null;


token_block::
token_block(std::vector<token>&&  c, operator_code  open, operator_code  close):
m_container(std::move(c)),
m_open(open),
m_close(close)
{
}




void
token_block::
clear() noexcept
{
  m_container.clear();

  m_open  = operator_code();
  m_close = operator_code();
}


void
token_block::
print() const noexcept
{
  printf("%s\n",m_open.get_string());

    for(auto&  tok: m_container)
    {
      tok.print();

      printf("\n");
    }


  printf("%s\n",m_close.get_string());
}




}




