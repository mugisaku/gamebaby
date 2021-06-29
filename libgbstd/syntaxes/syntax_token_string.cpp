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
syntax_token_string::
test_head_of_identifier(char16_t  c) noexcept
{
  return isalpha(c) || (c == '_');
}


bool
syntax_token_string::
test_body_of_identifier(char16_t  c) noexcept
{
  return isalnum(c) || (c == '_');
}


void
syntax_token_string::
read_string(source_code::iterator&  it)
{
  std::u16string  s;

    for(;;)
    {
      auto  c = *it;

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
          ++it;

          c = *it++;

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
          ++it;

          break;
        }

      else
        {
          s += c;

          ++it;
        }
    }


  m_buffer.emplace_back(std::move(s));
}


void
syntax_token_string::
read_raw_string(source_code::iterator&  it)
{
  std::u16string  s;

    for(;;)
    {
      auto  c = *it;

        if(c == '\0')
        {
          throw syntax_token_parse_error();
        }

      else
        if((c == '\"') &&
           (it[1] == ']') &&
           (it[2] == ']'))
        {
          it += 3;

          break;
        }

      else
        {
          s += c;

          ++it;
        }
    }


  m_buffer.emplace_back(std::move(s));
}


void
syntax_token_string::
step(source_code::iterator&  it)
{
  auto  c = *it;

  syntax_token_info  info(it);

    if(test_head_of_identifier(c))
    {
      std::u16string  s;

      s += c;

      ++it;

        while(test_body_of_identifier(*it))
        {
          s += *it++;
        }


      m_buffer.emplace_back(std::move(s),syntax_identifier());
    }

  else
    if((it[0] == '[') &&
       (it[1] == '[') &&
       (it[2] == '\"'))
    {
      it += 3;

      read_raw_string(it);
    }

  else
    if(c == '\"')
    {
      read_string(++it);
    }

  else
    if(isdigit(c))
    {
      read_number(it);
    }

  else
    if((it[0] == '/') &&
       (it[1] == '*'))
    {
      it += 2;

      it.skip_to_block_end();

      m_buffer.emplace_back(u' ');
    }

  else
    if((it[0] == '/') &&
       (it[1] == '/'))
    {
      it += 2;

      it.skip_to_newline();

      m_buffer.emplace_back(u' ');
    }

  else
    if(isspace(c))
    {
      ++it;

        while(isspace(*it))
        {
          ++it;
        }


      m_buffer.emplace_back((c == u'\n')? u'\n':u' ');
    }

  else
    {
      ++it;

      m_buffer.emplace_back(c);
    }


  m_buffer.back().add_info(std::move(info));
}


syntax_token_string&
syntax_token_string::
assign(const source_code&  src)
{
  source_code::iterator  it(src);

  m_buffer.clear();

    while(*it)
    {
      step(it);
    }


  m_buffer.emplace_back();

  return *this;
}




}




