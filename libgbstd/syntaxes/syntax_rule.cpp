#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_rule::
start_read(syntax_token_string::iterator&  it)
{
  auto&  name = it++->string();

  auto  f = find(name);

    if(!f)
    {
      f = &m_formula_list.emplace_back();
    }


  it.skip();

    if(it->is_others(u'='))
    {
      *f = read_formula(';',++it);

      f->set_name(name);
    }

  else
    if(it->is_others(u';'))
    {
      ++it;
    }

  else
    {
      ++it;

      report;
    }
}


syntax_rule::wrapper
syntax_rule::
read_formula_internal(syntax_token_string::iterator&  it)
{
    if(it->is_others(u'|') ||
       it->is_others(u'&') ||
       it->is_others(u':'))
    {
      return wrapper(static_cast<int>(it++->integer()));
    }

  else
    if(it->is_string())
    {
      return syntax_element(std::u16string_view(it++->string()));
    }

  else
    if(it->is_others(u'<'))
    {
      ++it;

        if(it[0].is_identifier() &&
           it[1].is_others(u'>'))
        {
          syntax_keyword  k(it->string());

          it += 2;

          return syntax_element(std::move(k));
        }
    }

  else
    if(it->is_others(u'('))
    {
      auto  f = read_formula(u')',++it);

      return syntax_element(std::move(f));
    }

  else
    if(it->is_others(u'['))
    {
      syntax_optional_formula  f(read_formula(u']',++it));

      return syntax_element(std::move(f));
    }

  else
    if(it->is_others(u'{'))
    {
      syntax_multiple_formula  f(read_formula(u'}',++it));

      return syntax_element(std::move(f));
    }

  else
    if(it->is_identifier())
    {
      auto&  name = it++->string();

        if((name == u"integer_literal") ||
           (name == u"floating_literal") ||
           (name == u"string_literal") ||
           (name == u"identifier"))
        {
          return syntax_element(std::u16string_view(name));
        }

      else
        {
          auto  f = find(name);

            if(!f)
            {
              f = &m_formula_list.emplace_back(name);
            }


          return syntax_element(syntax_reference(name));
        }
    }


  printf("read_formula_internal: 不正な要素");

  throw syntax_formula_error();
}


syntax_formula
syntax_rule::
make_formula(std::vector<wrapper>&&  stk)
{
  std::vector<syntax_element>  output;

    if(0)
    {
      printf("formula作成開始: %d\n",(int)stk.size());
    }


    for(auto&  wr: stk)
    {
        if(wr.is_code())
        {
            if(output.size() < 2)
            {
              printf("make_formula: 不正な要素数");

              throw syntax_formula_error();
            }


          auto  r = std::move(output.back());

          output.pop_back();

          auto  l = std::move(output.back());

          output.pop_back();

          syntax_formula  expr;

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
          printf("formula作成成功: ");
          e.print();
          print_nl();
        }


      return syntax_formula(std::move(e));
    }


  printf("formula作成失敗: %d\n",(int)output.size());

    for(auto&  e: output)
    {
      e.print();

      print_nl();
    }


  throw syntax_formula_error();
}


syntax_formula
syntax_rule::
read_formula(char16_t  close, syntax_token_string::iterator&  it)
{
  std::vector<wrapper>  stack;

  int  code = 0;

    for(;;)
    {
      it.skip();

        if(it->is_others(close))
        {
          ++it;

          break;
        }

      else
        if(!*it)
        {
          report;

          break;
        }

      else
        {
          auto  tmp_it = it;

            try
            {
              auto  wr = read_formula_internal(it);

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
                      tmp_it->print();
                    }


                  stack.emplace_back(std::move(wr));
                }
            }


            catch(syntax_formula_error&  e)
            {
              it->print();

              report;
              throw;
            }


            if(it == tmp_it)
            {
              printf("read_formula: ループに陥ったので停止した\n");

              tmp_it->print();

              throw syntax_formula_error();
            }
        }
    }


    if(code)
    {
      stack.emplace_back(code);
    }


  return stack.empty()? syntax_formula():make_formula(std::move(stack));
}


syntax_rule&
syntax_rule::
assign(const source_code&  src) noexcept
{
  m_formula_list.clear();

  return append(src);
}


syntax_rule&
syntax_rule::
append(const source_code&  src) noexcept
{
    if(0)
    {
      printf("定義を読み込み開始: ");

      print_nl();
    }


  syntax_token_string  toks(src);

  syntax_token_string::iterator  it(toks);

  it.skip();

    while(*it)
    {
        if(it->is_others(u'#'))
        {
          ++it;

            while(*it && !it->is_others(u'\n'))
            {
              ++it;
            }
        }

      else
        if(it->is_identifier())
        {
          start_read(it);
        }

      else
        {
          it->print();

          report;
          break;
        }


      it.skip();
    }


  return *this;
}


syntax_formula*
syntax_rule::
find(std::u16string_view  name) noexcept
{
    for(auto&  f: m_formula_list)
    {
        if(f.name() == name)
        {
          return &f;
        }
    }


  return nullptr;
}


const syntax_formula*
syntax_rule::
find(std::u16string_view  name) const noexcept
{
    for(auto&  f: m_formula_list)
    {
        if(f.name() == name)
        {
          return &f;
        }
    }


  return nullptr;
}


void
syntax_rule::
print() const noexcept
{
    for(auto&  f: m_formula_list)
    {
      f.print();

      printf(";\n");
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




