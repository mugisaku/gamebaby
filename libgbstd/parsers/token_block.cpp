#include"libgbstd/parser.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




const token
token_block_view::
m_null;


token_block::
token_block(std::string_view  sv) noexcept
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
print() const noexcept
{
  printf("%s\n",m_open.get_string());

    for(auto&  tok: m_container)
    {
      tok.print();

      auto    p = tok.get_begin();
      auto  end = tok.get_end();

      printf("  --  ");

        while(p < end)
        {
          printf("%c",*p++);
        }


      printf("  --\n");
    }


  printf("%s\n",m_close.get_string());
}




}




