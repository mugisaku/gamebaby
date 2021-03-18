#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_group::
print() const noexcept
{
    if(m_elements.empty())
    {
      return;
    }


  int   open = 0;
  int  close = 0;

    switch(m_kind)
    {
  case(kind::once    ):  open = '(';  close = ')';break;
  case(kind::optional):  open = '[';  close = ']';break;
  case(kind::multiple):  open = '{';  close = '}';break;
    }


  printf("%c",open);

    for(auto&  e: m_elements)
    {
      e.print();
    }


  printf("%c",close);
}


}




