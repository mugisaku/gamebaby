#include"libgbstd/character.hpp"




namespace gbstd{




text&
text::
assign(text&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_character_buffer,rhs.m_character_buffer);
      std::swap(m_lines,rhs.m_lines);
      std::swap(m_width,rhs.m_width);
      std::swap(m_height,rhs.m_height);
      std::swap(m_top_line    ,rhs.m_top_line);
      std::swap(m_bottom_line ,rhs.m_bottom_line);
      std::swap(m_current_line,rhs.m_current_line);
    }


  return *this;
}




text&
text::
clear() noexcept
{
  m_character_buffer.clear();
  m_lines.clear();

  m_top_line     = nullptr;
  m_bottom_line  = nullptr;
  m_current_line = nullptr;

  m_width  = 0;
  m_height = 0;

  return *this;
}


text&
text::
resize(int  w, int  h) noexcept
{
  clear();

  m_width  = w;
  m_height = h;

  m_character_buffer.resize(w*h);
  m_lines.resize(h);


  auto  p = m_character_buffer.data();

    for(int  y = 0;  y < h;  ++y)
    {
      auto&  ln = m_lines[y];

      ln.m_next = &m_lines[(y < (h-1))? (y+1):0];

      ln.m_buffer = p     ;
                    p += w;

      ln.m_length = 0;
    }


  m_top_line     = m_lines.data();
  m_bottom_line  = m_lines.data()+(h-1);
  m_current_line = m_lines.data();

  m_full_flag = false;

  return *this;
}


bool
text::
push(char16_t  c) noexcept
{
START:
    if(m_current_line->m_length < m_width)
    {
        if(c == '\n')
        {
          goto NEWLINE;
        }


      m_current_line->m_buffer[m_current_line->m_length++] = c;

      return true;
    }

  else
    if(m_current_line != m_bottom_line)
    {
NEWLINE:
      m_current_line = m_current_line->m_next;

      return true;
    }

  else
    {
      m_full_flag = true;
    }


  return false;
}


text&
text::
rotate() noexcept
{
  m_top_line     = m_top_line->m_next;
  m_bottom_line  = m_bottom_line->m_next;
  m_current_line = m_current_line->m_next;

  m_current_line->m_length = 0;

  m_full_flag = false;

  return *this;
}


const std::vector<std::u16string_view>&
text::
make_line_list() noexcept
{
  m_line_list.clear();

  auto  it = m_top_line;

    for(;;)
    {
      m_line_list.emplace_back(it->get_view());

        if(it == m_current_line)
        {
          break;
        }


      it = it->m_next;
    }


  return m_line_list;
}


void
text::
print() const noexcept
{
  printf("text{\n");

  utf8_encoder  enc;

  auto  it = m_top_line;

    for(;;)
    {
        for(auto  c: it->get_view())
        {
          printf("%s",enc(c).codes);
        }


        if(it == m_current_line)
        {
          break;
        }


      it = it->m_next;

      printf("\n");
    }


  printf("}\n");
}




}




