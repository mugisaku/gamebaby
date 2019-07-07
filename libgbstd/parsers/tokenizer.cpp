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


constexpr bool
isident0(char  c) noexcept
{
  return isalpha(c) || (c == '_');
}


constexpr bool
isidentn(char  c) noexcept
{
  return isalnum(c) || (c == '_');
}


}




token
tokenizer::
read_quoted_string(char  close_char)
{
  std::string  s;

    for(;;)
    {
      auto  c = *m_pointer++;

        if(c == close_char)
        {
          break;
        }

      else
        if(c == '\\')
        {
          c = *m_pointer++;
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
              printf("toknizer read_quoted_string error\n");

              throw 0;
            }
        }

      else
        if(!c)
        {
          printf("toknizer error: 文字列が%cで閉じられていない\n",close_char);

          throw 0;
        }


      s += c;
    }


  return token(m_info,std::move(s),close_char);
}


token
tokenizer::
read_operator_code() noexcept
{
  constexpr operator_code  codes[] = {
    operator_code("..."),
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
    operator_code("."),
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

        if(std::memcmp(m_pointer,s,l) == 0)
        {
          m_pointer += l;

          return token(m_info,opco);
        }
    }


  return token(m_info,operator_code());
}


token
tokenizer::
read_block(operator_code  open, operator_code  close)
{
  std::vector<token>  toks;

  auto  info = m_info;

  m_pointer += open.get_length();

  auto  close_len = close.get_length();

    for(;;)
    {
        if(!step(toks,close,close_len))
        {
          break;
        }


        if(m_pointer == info.get_pointer())
        {
          report;
          break;
        }
    }


  return token(info,token_block(std::move(toks),open,close));
}


bool
tokenizer::
step(std::vector<token>&  toks, operator_code  close, int close_len)
{
START:
  auto  c = *m_pointer;

    if((c ==  ' ') ||
       (c == '\n') ||
       (c == '\v') ||
       (c == '\t') ||
       (c == '\r'))
    {
      skip_spaces();

      c = *m_pointer;
    }


  update_info();

    if(!c)
    {
      return false;
    }

  else
    if(close_len && (std::memcmp(m_pointer,close.get_string(),close_len) == 0))
    {
      m_pointer += close_len;

      return false;
    }

  else
    if(isident0(c))
    {
      std::string  s;

        while(isidentn(*m_pointer))
        {
          s += *m_pointer++;
        }


      toks.emplace_back(m_info,std::move(s),'\0');

      return true;
    }

  else
    if(isdigit(c))
    {
      toks.emplace_back(read_number());

      return true;
    }

  else
    if((c == '\'') ||
       (c == '\"'))
    {
      ++m_pointer;

      toks.emplace_back(read_quoted_string(c));

      return true;
    }

  else
    if(std::memcmp(m_pointer,"/*",2) == 0)
    {
      m_pointer += 2;

      skip_blockstyle_comment();

      goto START;
    }

  else
    if(std::memcmp(m_pointer,"//",2) == 0)
    {
      m_pointer += 2;

      skip_linestyle_comment();

      goto START;
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
      toks.emplace_back(read_operator_code());

      return true;
    }

  else
    if(c == '(')
    {
      toks.emplace_back(read_block(operator_code("("),operator_code(")")));

      return true;
    }

  else
    if(c == '[')
    {
      toks.emplace_back(read_block(operator_code("["),operator_code("]")));

      return true;
    }

  else
    if(c == '{')
    {
      toks.emplace_back(read_block(operator_code("{"),operator_code("}")));

      return true;
    }

  else
    {
      printf("[処理できない文字です %d]\n",c);
      printf("[%d行目]\n",m_line_counter);
      printf("%s\n",m_pointer);

      throw 0;
    }


  return false;
}


token_block
tokenizer::
operator()(const char*  p)
{
  std::vector<token>  toks;

  m_pointer = p;

  m_line_counter = 0;

    for(;;)
    {
        if(!step(toks,"",0))
        {
          break;
        }


        if(m_pointer == p)
        {
          report;
          break;
        }
    }


  return token_block(std::move(toks),"","");
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




