#include"libgbstd/parser.hpp"
#include<cstring>
#include<cstdio>




namespace gbstd{


namespace{
constexpr bool
isalpha(char  c) noexcept
{
  return(((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')));
}


constexpr bool
isdigit(char  c) noexcept
{
  return((c >= '0') && (c <= '9'));
}


constexpr bool
isalnum(char  c) noexcept
{
  return(isalpha(c) || isdigit(c));
}


}




bool
tokenizer::
test_head_of_identifier_defaultly(char  c) noexcept
{
  return isalpha(c) || (c == '_');
}


bool
tokenizer::
test_body_of_identifier_defaultly(char  c) noexcept
{
  return isalnum(c) || (c == '_');
}




void
tokenizer::
read_quoted_string(char  close_char)
{
  std::string  s;

    while(m_current < m_end)
    {
      auto  c = *m_current++;

        if(c == close_char)
        {
          break;
        }

      else
        if(c == '\\')
        {
          c = *m_current++;

               if(!c){  report;printf("quoted string is not terminated by %c",close_char);  throw 0;}
          else if(c == '0'){c = '\0';}
          else if(c == 'n'){c = '\n';}
          else if(c == 't'){c = '\t';}
          else if(c == 'r'){c = '\r';}
          else if(c == 'b'){c = '\b';}
          else if(c == 'a'){c = '\a';}
          else if(c == 'f'){c = '\f';}
          else if(c == 'v'){c = '\v';}
          else if(c == '\\'){c = '\\';}
          else if(c == '\''){c = '\'';}
          else if(c == '\"'){c = '\"';}
/*
          else if(c == 'u'){c = 0;}
          else if(c == 'U'){c = 0;}
          else if(c == 'x'){c = 0;}
*/
          else
            {
              throw parse_error(get_line_number(),"Inkown escape sequence");
            }
        }

      else
        if(!c)
        {
          throw parse_error(get_line_number(),form_string("文字列が%cで閉じられていない",close_char));
        }


      s += c;
    }


  push(token(m_begin,m_current,get_line_number(),std::move(s),close_char));
}


void
tokenizer::
read_operator_code()
{
  constexpr operator_code  codes[] = {
    operator_code("..."),
    operator_code(".*"),
    operator_code("."),
    operator_code("<<="),
    operator_code("<<"),
    operator_code("<=>"),
    operator_code("<="),
    operator_code("<"),
    operator_code(">>="),
    operator_code(">>"),
    operator_code(">="),
    operator_code(">"),
    operator_code("++"),
    operator_code("+="),
    operator_code("+"),
    operator_code("--"),
    operator_code("-="),
    operator_code("->*"),
    operator_code("->"),
    operator_code("-"),
    operator_code("*="),
    operator_code("*"),
    operator_code("/="),
    operator_code("/"),
    operator_code("%="),
    operator_code("%"),
    operator_code("&&"),
    operator_code("&="),
    operator_code("&"),
    operator_code("||"),
    operator_code("|="),
    operator_code("|"),
    operator_code("^="),
    operator_code("^"),
    operator_code(":="),
    operator_code("::"),
    operator_code(":"),
    operator_code("==="),
    operator_code("=="),
    operator_code("="),
    operator_code("!=="),
    operator_code("!="),
    operator_code("!"),
    operator_code("?"),
    operator_code(","),
    operator_code(";"),
    operator_code("~"),
    operator_code("##"),
    operator_code("#"),
    operator_code("@"),
    operator_code("$"),
  };


    for(auto&  opco: codes)
    {
      auto  s = opco.get_string();

      auto  l = std::strlen(s);

        if(std::memcmp(m_current,s,l) == 0)
        {
          m_current += l;

          push(token(m_begin,m_current,get_line_number(),opco));

          return;
        }
    }


  throw parse_error(get_line_number(),"Unknown operator code");
}


void
tokenizer::
read_block(operator_code  open, operator_code  close) noexcept
{
  m_elements.emplace_back(m_begin,open,close);

  m_current += open.get_length();
}


void
tokenizer::
step(operator_code  close, int close_len)
{
  auto  c = *m_current;

    if(!c)
    {
      return;
    }

  else
    if(close_len && (std::memcmp(m_current,close.get_string(),close_len) == 0))
    {
      m_current += close_len;


      auto  e = std::move(m_elements.back());

      m_elements.pop_back();

      push(token(e.m_begin,m_current,get_line_number(),std::move(e.m_block)));

      return;
    }

  else
    if(m_test_head_of_identifier(c))
    {
      std::string  s;

        while(m_test_body_of_identifier(*m_current))
        {
          s += *m_current++;
        }


      push(token(m_begin,m_current,get_line_number(),std::move(s),'\0'));

      return;
    }

  else
    if(isdigit(c))
    {
      read_number();

      return;
    }

  else
    if((c == '\'') ||
       (c == '\"'))
    {
      ++m_current;

      read_quoted_string(c);

      return;
    }

  else
    if(std::memcmp(m_current,"/*",2) == 0)
    {
      m_current += 2;

      skip_blockstyle_comment();

      return;
    }

  else
    if(std::memcmp(m_current,"//",2) == 0)
    {
      m_current += 2;

      skip_linestyle_comment();

      return;
    }

  else
    if((c == '<') ||
       (c == '>') ||
       (c == '+') ||
       (c == '-') ||
       (c == '*') ||
       (c == '/') ||
       (c == '%') ||
       (c == '&') ||
       (c == '|') ||
       (c == '^') ||
       (c == ':') ||
       (c == '=') ||
       (c == '!') ||
       (c == '?') ||
       (c == ',') ||
       (c == '.') ||
       (c == ';') ||
       (c == '~') ||
       (c == '#') ||
       (c == '@') ||
       (c == '$'))
    {
      read_operator_code();

      return;
    }

  else
    if(c == '(')
    {
      read_block(operator_code("("),operator_code(")"));

      return;
    }

  else
    if(c == '[')
    {
      read_block(operator_code("["),operator_code("]"));

      return;
    }

  else
    if(c == '{')
    {
      read_block(operator_code("{"),operator_code("}"));

      return;
    }

  else
    {
      throw parse_error(get_line_number(),"処理できない文字");
    }
}


int
tokenizer::
get_line_number() const noexcept
{
  return m_line_number;
}


token_block
tokenizer::
operator()(std::string_view  sv)
{
  m_source_begin = sv.data();
  m_begin        = sv.data();
  m_current      = sv.data();
  m_end          = sv.data()+sv.size();

  m_line_number = 0;

  skip_spaces();

  m_elements.emplace_back(m_begin,"","");

    for(;;)
    {
        if(m_current < m_end)
        {
          auto&  blk = m_elements.back().m_block;

          m_begin = m_current;

          step(blk.get_close_code(),blk.get_close_code().get_length());

          skip_spaces();
        }

      else
        {
            if(m_elements.size() == 1)
            {
              return std::move(m_elements.back().m_block);
            }

          else
            {
              break;
            }
        }
    }


  throw parse_error(get_line_number(),"parse is failed");
}


void
print(const char*  start, const char*  cursor) noexcept
{
  int  line_number = 1;

  auto           p = start;
  auto  line_start = start;

    while(p < cursor)
    {
      auto  c = *p++;

        if(c == '\n')
        {
          line_start = cursor;

          ++line_number;
        }
    }


  printf("[stream %4d行]\n",line_number);

  p = line_start;

    while(*p && (*p != '\n'))
    {
      printf("%c",*p++);
    }


  printf("\n");


  p = line_start;

    while(p++ < (cursor-1))
    {
      printf(" ");
    }


  printf("[^]\n");
}


}




