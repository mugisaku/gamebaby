#include"libgbstd/syntax.hpp"




namespace gbstd{


namespace{
constexpr bool
isalpha(char16_t  c) noexcept
{
  return(((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')));
}


constexpr bool
isdigit(char16_t  c) noexcept
{
  return((c >= '0') && (c <= '9'));
}


constexpr bool
isalnum(char16_t  c) noexcept
{
  return(isalpha(c) || isdigit(c));
}


constexpr bool
isspace(char16_t  c) noexcept
{
  return((c ==  ' ') ||
         (c == '\n') ||
         (c == '\t') ||
         (c == '\r') ||
         (c == '\v'));
}


}




bool
syntax_tokenizer::
test_head_of_identifier_defaultly(char16_t  c) noexcept
{
  return isalpha(c) || (c == '_');
}


bool
syntax_tokenizer::
test_body_of_identifier_defaultly(char16_t  c) noexcept
{
  return isalnum(c) || (c == '_');
}


void
syntax_tokenizer::
read_string()
{
  std::u16string  s;

    for(;;)
    {
      auto  c = *m_iterator;

        if((c == '\0') ||
           (c == '\n') ||
           (c == '\r') ||
           (c == '\v'))
        {
          throw syntax_token_parse_error();
        }

      else
        if(c == '\\')
        {
          ++m_iterator;

          c = *m_iterator++;

               if(c == 'n'){c = '\n';}
          else if(c == 't'){c = '\t';}
          else if(c == '0'){c = '\0';}
          else if(c == 'r'){c = '\r';}
          else if(c == 'v'){c = '\r';}
          else if(c == '\\'){c = '\\';}

          s += c;
        }

      else
        if(c == '\"')
        {
          ++m_iterator;

          break;
        }

      else
        {
          s += c;

          ++m_iterator;
        }
    }


  m_buffer.emplace_back(std::move(s));
}


void
syntax_tokenizer::
read_raw_string()
{
  std::u16string  s;

    for(;;)
    {
      auto  c = *m_iterator;

        if(c == '\0')
        {
          throw syntax_token_parse_error();
        }

      else
        if((c == '\"') &&
           (m_iterator[1] == ']') &&
           (m_iterator[2] == ']'))
        {
          m_iterator += 3;

          break;
        }

      else
        {
          s += c;

          ++m_iterator;
        }
    }


  m_buffer.emplace_back(std::move(s));
}


void
syntax_tokenizer::
step()
{
  auto  c = *m_iterator;

  auto  x = m_iterator.get_x_index();
  auto  y = m_iterator.get_y_index();

    if(m_test_head_of_identifier(c))
    {
      std::u16string  s;

      s += c;

      ++m_iterator;

        while(m_test_body_of_identifier(*m_iterator))
        {
          s += *m_iterator++;
        }


      m_buffer.emplace_back(syntax_identifier(std::move(s)));
    }

  else
    if((m_iterator[0] == '[') &&
       (m_iterator[1] == '[') &&
       (m_iterator[2] == '\"'))
    {
      m_iterator += 3;

      read_raw_string();
    }

  else
    if(c == '\"')
    {
      ++m_iterator;

      read_string();
    }

  else
    if(isdigit(c))
    {
      read_number();
    }

  else
    if((m_iterator[0] == '/') &&
       (m_iterator[1] == '*'))
    {
      m_iterator += 2;

      m_iterator.skip_to_block_end();

      m_buffer.emplace_back(u' ');
    }

  else
    if((m_iterator[0] == '/') &&
       (m_iterator[1] == '/'))
    {
      m_iterator += 2;

      m_iterator.skip_to_newline();

      m_buffer.emplace_back(u' ');
    }

  else
    if(isspace(c))
    {
      ++m_iterator;

        while(isspace(*m_iterator))
        {
          ++m_iterator;
        }


      m_buffer.emplace_back((c == u'\n')? u'\n':u' ');
    }

  else
    {
      ++m_iterator;

      m_buffer.emplace_back(c);
    }


  m_buffer.back().set_point(x,y);
}


syntax_token_string
syntax_tokenizer::
operator()(std::u16string_view  sv)
{
  m_iterator.assign(sv.data());

  m_buffer.clear();

    while(*m_iterator)
    {
      step();
    }


  m_buffer.emplace_back();

  return std::move(m_buffer);
}


syntax_token_string
make_token_string(std::u16string_view  sv)
{
  syntax_tokenizer  tknz;

  return tknz(sv);
}


}




