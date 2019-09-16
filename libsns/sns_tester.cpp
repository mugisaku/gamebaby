#include"libsns/sns_record.hpp"




namespace sns{




namespace{
void
skip_spaces(const char*&  p) noexcept
{
    while(*p == ' ')
    {
      ++p;
    }
}


bool
search(const char*  word, int  l, const char*  content) noexcept
{
    while(*content)
    {
        if(std::strncmp(word,content,l) == 0)
        {
          return true;
        }


      ++content;
    }


  return false;
}


int
read_word(const char*&  cond, char*  p, int  l) noexcept
{
  int  n = 0;

    for(;;)
    {
      auto  c = *cond;

        if((c == '\0') ||
           (c == ' ') ||
           (c == '\n') ||
           (c == ')') ||
           (c == '|') ||
           (c == '&'))
        {
          *p = 0;

          return n;
        }

      else
        if(l > 1)
        {
          --l;

          ++cond;

          *p++ = c;

          ++n;
        }

      else
        {
          ++cond;
        }
    }
}


bool
test_word(const char*&  cond, const char*  content, bool  ignore) noexcept
{
  char  buf[256];

  int  l = read_word(cond,buf,sizeof(buf));

//printf("word = \"%s\"\n",buf);
  return ignore? false:search(buf,l,content);
}


bool  evaluate(const char*&  cond, const char*  content, bool  ignore) noexcept;


bool
test_block(const char*&  cond, char  close_char, const char*  content, bool  ignore) noexcept
{
  bool  result = true;

    for(;;)
    {
      auto  v = evaluate(cond,content,ignore);

      skip_spaces(cond);

      auto  c = *cond++;

        if(c == close_char)
        {
          return v;
        }

      else
        if(c == '|')
        {
            if(v)
            {
              result = true;

              goto QUIT;
            }
        }

      else
        if(c == '&')
        {
            if(!v)
            {
              result = false;

              goto QUIT;
            }
        }
    }


QUIT:
    for(;;)
    {
      auto  c = *cond++;

        if(c == close_char)
        {
          return result;
        }
    }
}


bool
evaluate(const char*&  cond, const char*  content, bool  ignore) noexcept
{
  bool  not_flag = false;

  skip_spaces(cond);

  auto  c = *cond;

    if(c == '!')
    {
//printf("not_flag was set\n");

      c = *++cond;

      not_flag = true;
    }


  bool  res;

    if(c == '(')
    {
      ++cond;

      res = test_block(cond,')',content,ignore);
    }

  else
    {
      res = test_word(cond,content,ignore);
    }


    if(not_flag)
    {
      res = !res;
    }


  return res;
}
}



bool
tester::
operator()(const char*  content) const noexcept
{
  auto  p = m_condition;

  return test_block(p,'\0',content,false);
}




}




