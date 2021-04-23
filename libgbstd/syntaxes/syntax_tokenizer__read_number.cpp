#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_tokenizer::
read_binary_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_iterator;

        if((c == '0') ||
           (c == '1'))
        {
          n <<= 1;

            if(c == '1')
            {
              n |= 1;
            }


          ++m_iterator;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(n);
}


void
syntax_tokenizer::
read_octal_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_iterator;

        if((c >= '0') &&
           (c <= '7'))
        {
          n <<= 2;
          n  |= c-'0';

          ++m_iterator;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(n);
}


void
syntax_tokenizer::
read_decimal_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_iterator;

        if((c >= '0') &&
           (c <= '9'))
        {
          n *=    10;
          n += c-'0';

          ++m_iterator;
        }

      else
        {
          break;
        }
    }


    if(*m_iterator == '.')
    {
      ++m_iterator;

      read_floating_point_number(n);

      return;
    }

  else
    {
      m_buffer.emplace_back(n);
    }
}


void
syntax_tokenizer::
read_hexadecimal_number() noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *m_iterator;

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


          ++m_iterator;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(n);
}


void
syntax_tokenizer::
read_floating_point_number(uint64_t  i) noexcept
{
  auto  fpn = static_cast<double>(i);

  double  fra = 0.1;

    for(;;)
    {
      auto  c = *m_iterator;

        if((c >= '0') &&
           (c <= '9'))
        {
          fpn += fra*(c-'0');
          fra /= 10;

          ++m_iterator;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(fpn);
}


void
syntax_tokenizer::
read_number_that_begins_by_zero() noexcept
{
  auto  c = *++m_iterator;

       if((c == 'b') || (c == 'B')){  ++m_iterator;           read_binary_number();}
  else if((c == 'o') || (c == 'O')){  ++m_iterator;            read_octal_number();}
  else if((c == 'x') || (c == 'X')){  ++m_iterator;      read_hexadecimal_number();}
  else if((c == '.')              ){  ++m_iterator;  read_floating_point_number(0);}
  else                             {m_buffer.emplace_back(static_cast<uint64_t>(0));}
}


void
syntax_tokenizer::
read_number() noexcept
{
    if(*m_iterator == '0'){read_number_that_begins_by_zero();}
  else                    {            read_decimal_number();}
}


}




