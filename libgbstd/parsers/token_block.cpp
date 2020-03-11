#include"libgbstd/parser.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




const token
token_iterator::
m_null;


token_block::
token_block(std::string_view  sv)
{
  tokenizer  tknz;

  *this = tknz(sv);
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
print(const char*  base, int  indent) const noexcept
{
  printf("%s\n",m_open.get_string());

    for(auto&  tok: m_container)
    {
      tok.print(base,indent);

      printf("\n");
    }


    for(int  n = 0;  n < indent;  ++n)
    {
      printf(" ");
    }


  printf("%s\n",m_close.get_string());
}




}




