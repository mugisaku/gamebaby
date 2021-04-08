#include"libgbstd/character.hpp"
#include<cstdio>




namespace gbstd{




code_text&
code_text::
append(std::string_view  sv) noexcept
{
  m_string = make_u16string(sv);

  auto  p = m_string.data();

  m_line_heads.emplace_back(p);

    while(*p)
    {
      auto  c = *p++;

        if(c == '\n')
        {
          m_line_heads.emplace_back(p);
        }
    }


  return *this;
}


code_text&
code_text::
assign(std::string_view  sv) noexcept
{
  m_line_heads.clear();

  return append(sv);
}


void
code_text::
print() const noexcept
{
  gbstd::print(m_string);

  printf("\n");
}




code_text::iterator&
code_text::iterator::
operator++() noexcept
{
  auto  c = *m_pointer++;

    if(c == '\n')
    {
      m_x_index  = 0;
      m_y_index += 1;
    }

  else
    {
      ++m_x_index;
    }


  return *this;
}


code_text::iterator
code_text::iterator::
operator++(int) noexcept
{
  auto  tmp = *this;

  ++(*this);

  return tmp;
}


code_text::iterator&
code_text::iterator::
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
code_text::iterator::
print() const noexcept
{
  printf("{%4d行目, %4d文字目}",1+m_y_index,1+m_x_index);
}


void
code_text::iterator::
skip_spaces() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if((c == ' ') ||
           (c == '\t'))
        {
          ++m_pointer;
          ++m_x_index;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_index  = 0;
          m_y_index += 1;
        }

      else
        {
          break;
        }
    }
}




}




