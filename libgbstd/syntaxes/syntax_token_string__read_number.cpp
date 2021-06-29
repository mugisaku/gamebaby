#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_token_string::
read_binary_number(source_code::iterator&  it) noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *it;

        if((c == '0') ||
           (c == '1'))
        {
          n <<= 1;

            if(c == '1')
            {
              n |= 1;
            }


          ++it;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(n);
}


void
syntax_token_string::
read_octal_number(source_code::iterator&  it) noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *it;

        if((c >= '0') &&
           (c <= '7'))
        {
          n <<= 2;
          n  |= c-'0';

          ++it;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(n);
}


void
syntax_token_string::
read_decimal_number(source_code::iterator&  it) noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *it;

        if((c >= '0') &&
           (c <= '9'))
        {
          n *=    10;
          n += c-'0';

          ++it;
        }

      else
        {
          break;
        }
    }


    if(*it == '.')
    {
      ++it;

      read_floating_point_number(it,n);

      return;
    }

  else
    {
      m_buffer.emplace_back(n);
    }
}


void
syntax_token_string::
read_hexadecimal_number(source_code::iterator&  it) noexcept
{
  uint64_t  n = 0;

    for(;;)
    {
      auto  c = *it;

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


          ++it;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(n);
}


void
syntax_token_string::
read_floating_point_number(source_code::iterator&  it, uint64_t  i) noexcept
{
  auto  fpn = static_cast<double>(i);

  double  fra = 0.1;

    for(;;)
    {
      auto  c = *it;

        if((c >= '0') &&
           (c <= '9'))
        {
          fpn += fra*(c-'0');
          fra /= 10;

          ++it;
        }

      else
        {
          break;
        }
    }


  m_buffer.emplace_back(fpn);
}


void
syntax_token_string::
read_number_that_begins_by_zero(source_code::iterator&  it) noexcept
{
  auto  c = *++it;

       if((c == 'b') || (c == 'B')){        read_binary_number(++it);}
  else if((c == 'o') || (c == 'O')){         read_octal_number(++it);}
  else if((c == 'x') || (c == 'X')){   read_hexadecimal_number(++it);}
  else if((c == '.')              ){read_floating_point_number(++it,0);}
  else                             {m_buffer.emplace_back(static_cast<uint64_t>(0));}
}


void
syntax_token_string::
read_number(source_code::iterator&  it) noexcept
{
    if(*it == '0'){read_number_that_begins_by_zero(it);}
  else            {            read_decimal_number(it);}
}


}




