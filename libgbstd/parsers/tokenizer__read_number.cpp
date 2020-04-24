#include"libgbstd/parser.hpp"




namespace gbstd{




void
tokenizer::
read_binary_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_current;

        if((c == '0') ||
           (c == '1'))
        {
          n <<= 1;

            if(c == '1')
            {
              n |= 1;
            }


          ++m_current;
        }

      else
        {
          break;
        }
    }


  push(token(m_begin,m_current,get_line_number(),n));
}


void
tokenizer::
read_octal_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_current;

        if((c >= '0') &&
           (c <= '7'))
        {
          n <<= 2;
          n  |= c-'0';

          ++m_current;
        }

      else
        {
          break;
        }
    }


  push(token(m_begin,m_current,get_line_number(),n));
}


void
tokenizer::
read_decimal_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_current;

        if((c >= '0') &&
           (c <= '9'))
        {
          n *=    10;
          n += c-'0';

          ++m_current;
        }

      else
        {
          break;
        }
    }


    if(*m_current == '.')
    {
      ++m_current;

      read_floating_point_number(n);

      return;
    }

  else
    {
      push(token(m_begin,m_current,get_line_number(),n));
    }
}


void
tokenizer::
read_hexadecimal_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_current;

        if(((c >= '0') && (c <= '9')) ||
           ((c >= 'a') && (c <= 'f')) ||
           ((c >= 'A') && (c <= 'F')))
        {
          n <<= 4;

            switch(c)
            {
          case('0'):
          case('1'):
          case('2'):
          case('3'):
          case('4'):
          case('5'):
          case('6'):
          case('7'):
          case('8'):
          case('9'):
              n |= c-'0';
              break;
          case('a'):
          case('A'):
              n |= 0xA;
              break;
          case('b'):
          case('B'):
              n |= 0xB;
              break;
          case('c'):
          case('C'):
              n |= 0xC;
              break;
          case('d'):
          case('D'):
              n |= 0xD;
              break;
          case('e'):
          case('E'):
              n |= 0xE;
              break;
          case('f'):
          case('F'):
              n |= 0xF;
              break;
            }


          ++m_current;
        }

      else
        {
          break;
        }
    }


  push(token(m_begin,m_current,get_line_number(),n));
}


void
tokenizer::
read_floating_point_number(uint64_t  i) noexcept
{
  auto  fpn = static_cast<double>(i);

  double  fra = 0.1;

    for(;;)
    {
      auto  c = *m_current;

        if((c >= '0') &&
           (c <= '9'))
        {
          fpn += fra*(c-'0');
          fra /= 10;

          ++m_current;
        }

      else
        {
          break;
        }
    }


  push(token(m_begin,m_current,get_line_number(),fpn));
}


void
tokenizer::
read_number_that_begins_by_zero() noexcept
{
  auto  c = *++m_current;

       if((c == 'b') || (c == 'B')){  ++m_current;           read_binary_number();}
  else if((c == 'o') || (c == 'O')){  ++m_current;            read_octal_number();}
  else if((c == 'x') || (c == 'X')){  ++m_current;      read_hexadecimal_number();}
  else if((c == '.')              ){  ++m_current;  read_floating_point_number(0);}
  else                             {push(token(m_begin,m_current,get_line_number(),static_cast<uint64_t>(0)));}
}


void
tokenizer::
read_number() noexcept
{
    if(*m_current == '0'){read_number_that_begins_by_zero();}
  else                   {            read_decimal_number();}
}


}




