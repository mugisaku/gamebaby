#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_rule::
start_read(code_text::iterator&  it)
{
  auto  name = read_identifier(it);

  auto  def = find(name);

    if(!def)
    {
      def = &m_definition_list.emplace_back(name);
    }


  it.skip_spaces();

  auto  c = *it;

    if(c == '=')
    {
      static_cast<syntax_definition&>(*def).assign(read_expression(';',++it));
    }

  else
    if(c == ';')
    {
      ++it;
    }

  else
    {
      ++it;

      report;
    }
}


syntax_keyword
syntax_rule::
read_keyword(code_text::iterator&  it)
{
  std::u16string  s;

    while(*it)
    {
      auto  c = *it;

        if(is_alphabet(c) || (c == '_'))
        {
          s += c;

          ++it;
        }

      else
        if(c == '>')
        {
          ++it;

          break;
        }

      else
        {
          printf("read_keyword: 不正な要素");

          throw syntax_expression_error();
        }
    }


  return syntax_keyword(std::move(s));
}


std::u16string
syntax_rule::
read_identifier(code_text::iterator&  it)
{
  std::u16string  s;

    while(*it)
    {
      auto  c = *it;

        if(is_alphabet(c) || (c == '_'))
        {
          s += c;

          ++it;
        }

      else
        {
          break;
        }
    }


  return std::move(s);
}


std::u16string
syntax_rule::
read_string(code_text::iterator&  it)
{
  std::u16string  s;

    while(*it)
    {
      auto  c = *it;

        if(c == '\"')
        {
          ++it;

          break;
        }

      else
        if(c == '\\')
        {
          ++it;

          c = *it;

               if(c ==  'n'){s += '\n';}
          else if(c ==  'r'){s += '\r';}
          else if(c ==  'v'){s += '\v';}
          else if(c ==  't'){s += '\t';}
          else if(c == '\\'){s += '\\';}
          else if(c ==  '0'){s += '\0';}
          else if(c ==  '\"'){s += '\"';}
          else if(c ==  '\''){s += '\'';}
          else{report;}

          ++it;
        }

      else
        {
          ++it;

          s += c;
        }
    }


  return std::move(s);
}


syntax_rule::wrapper
syntax_rule::
read_expression_internal(code_text::iterator&  it)
{
  auto  c = *it;

    if((c == '|') ||
       (c == '&') ||
       (c == ':'))
    {
      ++it;

      return wrapper(c);
    }

  else
    if(c == '\"')
    {
      return syntax_expression_element(read_string(++it));
    }

  else
    if(c == '<')
    {
      return syntax_expression_element(read_keyword(++it));
    }

  else
    if(c == '(')
    {
      auto  e = read_expression(')',++it);

      return syntax_expression_element(std::move(e));
    }

  else
    if(c == '[')
    {
      syntax_optional_expression  e(read_expression(']',++it));

      return syntax_expression_element(std::move(e));
    }

  else
    if(c == '{')
    {
      syntax_multiple_expression  e(read_expression('}',++it));

      return syntax_expression_element(std::move(e));
    }

  else
    {
      auto  name = read_identifier(it);

      auto  def = find(name);

        if(!def)
        {
          def = &m_definition_list.emplace_back(name);
        }


      return syntax_expression_element(*def);
    }


  printf("read_expression_internal: 不正な要素");

  throw syntax_expression_error();
}


syntax_expression
syntax_rule::
make_expression(std::vector<wrapper>&&  stk)
{
  std::vector<syntax_expression_element>  output;

    if(0)
    {
      printf("expression作成開始: %d\n",(int)stk.size());
    }


    for(auto&  wr: stk)
    {
        if(wr.is_code())
        {
            if(output.size() < 2)
            {
              printf("make_expression: 不正な要素数");

              throw syntax_expression_error();
            }


          auto  r = std::move(output.back());

          output.pop_back();

          auto  l = std::move(output.back());

          output.pop_back();

          syntax_expression  expr;

          expr.set_code(wr.code())
              .set_left(std::move(l))
              .set_right(std::move(r))
              ;

          output.emplace_back(std::move(expr));
        }

      else
        {
          output.emplace_back(std::move(wr.element()));
        }
    }


    if(output.size() == 1)
    {
      auto&  e = output.back();

        if(0)
        {
          printf("expression作成成功: ");
          e.print();
          print_nl();
        }


      return syntax_expression(std::move(e));
    }


  printf("expression作成失敗: %d\n",(int)output.size());

    for(auto&  e: output)
    {
      e.print();

      print_nl();
    }


  throw syntax_expression_error();
}


syntax_expression
syntax_rule::
read_expression(int  close, code_text::iterator&  it)
{
  std::vector<wrapper>  stack;

  int  code = 0;

    for(;;)
    {
      it.skip_spaces();

      auto  c = *it;

        if(c == close)
        {
          ++it;

          break;
        }

      else
        if(!c)
        {
          report;

          break;
        }

      else
        {
          auto  tmp_it = it;

            try
            {
              auto  wr = read_expression_internal(it);

                if(wr.is_code())
                {
                    if(code)
                    {
                      stack.emplace_back(code);
                    }


                  code = wr.code();
                }

              else
                {
                    if(wr.element().is_null())
                    {
                      tmp_it.print();
                    }

                  stack.emplace_back(std::move(wr));
                }
            }


            catch(syntax_expression_error&  e)
            {
              it.print();

              report;
              throw;
            }


            if(it == tmp_it)
            {
              printf("read_expression: ループに陥ったので停止した\n");

              tmp_it.print();

              throw syntax_expression_error();
            }
        }
    }


    if(code)
    {
      stack.emplace_back(code);
    }


  return stack.empty()? syntax_expression():make_expression(std::move(stack));
}


syntax_rule&
syntax_rule::
assign(std::string_view  sv) noexcept
{
  m_definition_list.clear();

  return append(sv);
}


syntax_rule&
syntax_rule::
assign(std::u16string_view  sv) noexcept
{
  m_definition_list.clear();

  return append(sv);
}


syntax_rule&
syntax_rule::
append(std::string_view  sv) noexcept
{
  auto  u16s = make_u16string(sv);

  return append(u16s);
}


syntax_rule&
syntax_rule::
append(std::u16string_view  sv) noexcept
{
    if(0)
    {
      printf("定義を読み込み開始: ");
      gbstd::print(sv);
      print_nl();
    }


  code_text::iterator  it(sv.data());

  it.skip_spaces();

    while(*it)
    {
      auto  c = *it;

        if(is_alphabet(c) || (c == '_'))
        {
          start_read(it);

          it.skip_spaces();
        }

      else
        {
          printf("<%c>\n",c);

          report;
          break;
        }
    }


  return *this;
}


syntax_definition*
syntax_rule::
find(std::u16string_view  name) noexcept
{
    for(auto&  def: m_definition_list)
    {
        if(def.get_name() == name)
        {
          return &def;
        }
    }


  return nullptr;
}


const syntax_definition*
syntax_rule::
find(std::u16string_view  name) const noexcept
{
    for(auto&  def: m_definition_list)
    {
        if(def.get_name() == name)
        {
          return &def;
        }
    }


  return nullptr;
}


void
syntax_rule::
print() const noexcept
{
    for(auto&  def: m_definition_list)
    {
      def.print();

      printf("\n");
    }
}


void
syntax_rule::wrapper::
print() const noexcept
{
    if(is_code())
    {
      printf("code: %c",m_code);
    }

  else
    {
      printf("element: ");

      m_element.print();
    }
}



}




