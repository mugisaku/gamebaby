#include"libgbstd/character.hpp"




namespace gbstd{




text::node*
text::
m_stock_pointer;




text&
text::
assign(text&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_top_pointer      ,rhs.m_top_pointer);
      std::swap(m_bottom_pointer   ,rhs.m_bottom_pointer);
      std::swap(m_current_pointer  ,rhs.m_current_pointer);
      std::swap(m_character_counter,rhs.m_character_counter);
      std::swap(m_number_of_lines  ,rhs.m_number_of_lines);
    }


  return *this;
}




text&
text::
clear() noexcept
{
  auto  ptr = m_top_pointer;

  m_top_pointer     = nullptr;
  m_bottom_pointer  = nullptr;
  m_current_pointer = nullptr;

    while(ptr)
    {
      auto  next = ptr->m_next;
                   ptr->m_next = m_stock_pointer      ;
                                 m_stock_pointer = ptr;

      ptr = next;
    }


  m_character_counter = 0;
  m_number_of_lines   = 0;

  return *this;
}


text::node*
text::
new_node() noexcept
{
    if(!m_stock_pointer)
    {
      m_stock_pointer = new node;

      m_stock_pointer->m_next = nullptr;
    }


  auto  nd = m_stock_pointer                          ;
             m_stock_pointer = m_stock_pointer->m_next;

    if(m_bottom_pointer)
    {
      m_bottom_pointer->m_next = nd;
    }

  else
    {
      m_top_pointer     = nd;
      m_current_pointer = nd;
    }


  m_bottom_pointer = nd;

  nd->m_next = nullptr;

  nd->m_line.m_content_length = 0;
  nd->m_line.m_display_length = 0;

  ++m_number_of_lines;

  return nd;
}


text&
text::
push(std::string_view  sv) noexcept
{
  auto  nd = new_node();

  utf8_decoder  dec(sv.data());

    while(dec)
    {
        if(nd->m_line.is_full())
        {
          nd = new_node();
        }


      auto  c = dec();

        if(c == '\n')
        {
          nd = new_node();
        }

      else
        {
          nd->m_line.push(c);

          ++m_character_counter;
        }
    }


  return *this;
}


text&
text::
push(std::u16string_view  sv) noexcept
{
  auto  nd = new_node();

    for(auto  c: sv)
    {
        if(nd->m_line.is_full())
        {
          nd = new_node();
        }


        if(c == '\n')
        {
          nd = new_node();
        }

      else
        {
          nd->m_line.push(c);

          ++m_character_counter;
        }
    }


  return *this;
}


text&
text::
pop() noexcept
{
    if(m_top_pointer)
    {
      m_character_counter -= m_top_pointer->m_line.m_display_length;

        if(m_current_pointer == m_top_pointer)
        {
          m_current_pointer = m_current_pointer->m_next;
        }


      auto  nd = m_top_pointer                        ;
                 m_top_pointer = m_top_pointer->m_next;

      nd->m_next = m_stock_pointer     ;
                   m_stock_pointer = nd;

        if(!--m_number_of_lines)
        {
          m_bottom_pointer = nullptr;
        }
    }


  return *this;
}


bool
text::
expose_one_character() noexcept
{
START:
    if(m_current_pointer)
    {
      auto&  ln = m_current_pointer->m_line;

        if(!ln.is_displaying_all())
        {
          ++ln.m_display_length;

          --m_character_counter;

          return true;
        }

      else
        if(m_current_pointer->m_next)
        {
          m_current_pointer = m_current_pointer->m_next;

          goto START;
        }
    }


  return false;
}


bool
text::
expose_all_characters_of_current_line() noexcept
{
START:
    if(m_current_pointer)
    {
      auto&  ln = m_current_pointer->m_line;

        if(!ln.is_displaying_all())
        {
          m_character_counter -= ln.m_content_length-ln.m_display_length;

          ln.m_display_length = ln.m_content_length;

          return true;
        }

      else
        if(m_current_pointer->m_next)
        {
          m_current_pointer = m_current_pointer->m_next;

          goto START;
        }
    }


  return false;
}


void
text::
print() const noexcept
{
  printf("text{\n");

    for(auto&  ln: *this)
    {
      utf8_encoder  enc;

        for(auto  c: ln)
        {
          printf("%s",enc(c).codes);
        }


      printf("\n");
    }


  printf("}\n");
}




text::iterator
text::iterator::
operator++(int) noexcept
{
  auto  tmp = *this;

  m_pointer = m_pointer->m_next;

  return tmp;
}


text::iterator
text::iterator::
operator+(int  n) const noexcept
{
  auto  ptr = m_pointer;

    while(n--)
    {
      ptr = ptr->m_next;
    }


  return iterator(ptr);
}


text::iterator&
text::iterator::
operator+=(int  n) noexcept
{
    while(n--)
    {
      m_pointer = m_pointer->m_next;
    }


  return *this;
}


}




