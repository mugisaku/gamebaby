#include"libgbstd/file_op.hpp"
#include"libgbstd/utility.hpp"




namespace gbstd{




source_code::iterator&
source_code::iterator::
assign(const char16_t*  p) noexcept
{
  m_source.unrefer();

  m_x_position = 0;
  m_y_position = 0;

  return *this;
}


source_code::iterator&
source_code::iterator::
assign(const source_code&  src) noexcept
{
  m_source = src;

  m_pointer = src.content().data();

  m_x_position = 0;
  m_y_position = 0;

  return *this;
}


source_code::iterator
source_code::iterator::
operator+(int  n) const noexcept
{
  auto  t = *this;

  t += n;

  return t;
}


source_code::iterator&
source_code::iterator::
operator++() noexcept
{
  auto  c = *m_pointer++;

    if(c == '\n')
    {
      m_x_position  = 0;
      m_y_position += 1;
    }

  else
    {
      ++m_x_position;
    }


  return *this;
}


source_code::iterator
source_code::iterator::
operator++(int) noexcept
{
  auto  tmp = *this;

  ++(*this);

  return tmp;
}


source_code::iterator&
source_code::iterator::
operator+=(int  n) noexcept
{
    if(n > 0)
    {
        while(n--)
        {
          ++(*this);
        }
    }


  return *this;
}




void
source_code::iterator::
print() const noexcept
{
  printf("{%4d行目, %4d文字目}",1+m_y_position,1+m_x_position);
}


void
source_code::iterator::
skip_spaces() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if((c == ' ') ||
           (c == '\t'))
        {
          ++m_pointer;
          ++m_x_position;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_position  = 0;
          m_y_position += 1;
        }

      else
        {
          break;
        }
    }
}


void
source_code::iterator::
skip_to_newline() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if(c == '\0')
        {
          break;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_position  = 0;
          m_y_position += 1;

          break;
        }

      else
        {
          ++m_pointer;
          ++m_x_position;
        }
    }
}


void
source_code::iterator::
skip_to_block_end() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if(c == '\0')
        {
          report;

          break;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_position  = 0;
          m_y_position += 1;
        }

      else
        {
          ++m_pointer;
          ++m_x_position;

            if((c == '*') && (*m_pointer == '/'))
            {
              ++m_pointer;
              ++m_x_position;

              break;
            }
        }
    }
}




}




