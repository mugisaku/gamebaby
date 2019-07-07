#include"libgbstd/parser.hpp"
#include<cstring>




namespace gbstd{


void
tokenizer::
skip_linestyle_comment() noexcept
{
    for(;;)
    {
      auto  c = *m_pointer;

        if(!c)
        {
          break;
        }

      else           
        if(c == '\n')
        {
          ++m_line_counter;

          ++m_pointer;

          break;
        }

      else
        {
          ++m_pointer;
        }           
    }
}


void
tokenizer::
skip_blockstyle_comment()
{
    for(;;)
    {
      auto  c = *m_pointer;

        if(!c)
        {
          printf("ブロック式コメントが閉じられていない\n");

          throw m_pointer;
        }

      else           
        if(c == '*')
        {
          ++m_pointer;

            if(*m_pointer == '/')
            {
              ++m_pointer;

              break;
            }
        }

      else
        if(c == '\n')
        {
          ++m_line_counter;
          ++m_pointer;
        }

      else
        {
          ++m_pointer;
        }           
    }
}


void
tokenizer::
skip_spaces()
{
    for(;;)
    {
      auto  c = *m_pointer;

        if(!c)
        {
          break;
        }

      else
        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r') ||
           (c == '\v'))
        {
          ++m_pointer;
        }

      else
        if(c == '\n')
        {
          ++m_line_counter;
          ++m_pointer;
        }

      else           
        if(c == '/')
        {
          c = *(m_pointer+1);

            if(c == '*')
            {
              m_pointer += 2;

              skip_blockstyle_comment();
            }

          else
            if(c == '/')
            {
              m_pointer += 2;

              skip_linestyle_comment();
            }

          else
            {
              break;
            }
        }

      else
        {
          break;
        }           
    }
}


}




