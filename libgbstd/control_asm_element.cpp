#include"libgbstd/gbstd.hpp"




namespace gbstd{




void
asm_element::
print() const noexcept
{
    switch(m_kind)
    {
  case(asm_kind::null  ): printf("  null");break;
  case(asm_kind::invoke): printf("  \"%s\"",m_first.data());break;
  case(asm_kind::jz    ): printf(" *!\"%s\",\"%s\"",m_first.data(),m_second.data());break;
  case(asm_kind::jnz   ): printf(" * \"%s\",\"%s\"",m_first.data(),m_second.data());break;
  case(asm_kind::label ): printf("\"%s\":",m_first.data());break;
    }


    if(m_interrupt_flag)
    {
      printf(".");
    }
}


}




