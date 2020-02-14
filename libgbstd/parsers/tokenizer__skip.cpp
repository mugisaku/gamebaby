#include"libgbstd/parser.hpp"
#include<cstring>




namespace gbstd{


void
tokenizer::
skip_linestyle_comment() noexcept
{
    while(m_current < m_end)
    {
      auto  c = *m_current;

        if(!c)
        {
          break;
        }

      else           
        if(c == '\n')
        {
          ++m_current;

          break;
        }

      else
        {
          ++m_current;
        }           
    }
}


void
tokenizer::
skip_blockstyle_comment()
{
    while(m_current < m_end)
    {
      auto  c = *m_current;

        if(!c)
        {
          printf("ブロック式コメントが閉じられていない\n");

          throw m_current;
        }

      else           
        if(c == '*')
        {
          ++m_current;

            if(*m_current == '/')
            {
              ++m_current;

              break;
            }
        }

      else
        {
          ++m_current;
        }           
    }
}


void
tokenizer::
skip_spaces()
{
    while(m_current < m_end)
    {
      auto  c = *m_current;

        if(!c)
        {
          break;
        }

      else
        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r') ||
           (c == '\n') ||
           (c == '\v'))
        {
          ++m_current;
        }

      else           
        if(c == '/')
        {
          c = *(m_current+1);

            if(c == '*')
            {
              m_current += 2;

              skip_blockstyle_comment();
            }

          else
            if(c == '/')
            {
              m_current += 2;

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




