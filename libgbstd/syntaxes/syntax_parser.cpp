#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_parser::result::
result(result&&  l, result&&  r) noexcept:
m_iterator(r.m_iterator), m_elements(std::move(l.m_elements))
{
    for(auto&  e: r.release())
    {
      m_elements.emplace_back(std::move(e));
    }
}




syntax_parser::result
syntax_parser::
process_or(const syntax_element&  l, const syntax_element&  r, syntax_token_string::iterator  it)
{
    if(m_debugging)
    {
      printf("or判定を開始\n");
    }


  auto  res = process_by_element(l,it);

    if(res)
    {
        if(m_debugging)
        {
          printf("左辺を採用\n");
        }


      return std::move(res);
    }


  res = process_by_element(r,it);

    if(res)
    {
        if(m_debugging)
        {
          printf("右辺を採用\n");
        }


      return std::move(res);
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_and(const syntax_element&  l, const syntax_element&  r, syntax_token_string::iterator  it)
{
    if(m_debugging)
    {
      printf("and判定を開始\n");
    }


  auto  lres = process_by_element(l,it);

    if(lres)
    {
      auto  rres = process_by_element(r,lres.iterator());

        if(rres)
        {
          return result(std::move(lres),std::move(rres));
        }
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_colon(const syntax_formula&  f, syntax_token_string::iterator  it)
{
    if(m_debugging)
    {
      printf("colon判定を開始\n");
    }


  auto  lres = process_by_element(*f.left(),it);

    if(lres)
    {
      m_point_stack.emplace_back(it,f);

      auto  rres = process_by_element(*f.right(),lres.iterator());

        if(rres)
        {
          return result(std::move(lres),std::move(rres));
        }


//      printf(":後の解析失敗");

//      it->print();

//      throw syntax_parse_error();
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_by_formula(const syntax_formula&  f, syntax_token_string::iterator  it)
{
  auto  code = f.code();
  auto&    l = f.left();
  auto&    r = f.right();

       if(code == '|'){return process_or( *l,*r,it);}
  else if(code == '&'){return process_and(*l,*r,it);}
  else if(code == ':'){return process_colon(f,it);}
  else
    if(l)
    {
      auto  res = process_by_element(*l,it);

        if(res)
        {
          return std::move(res);
        }
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_by_named_formula(const syntax_formula&  f, syntax_token_string::iterator  it)
{
    if(m_debugging)
    {
      gbstd::print(f.name());
      printf("による解析を開始\n");
    }


  constexpr int  depth_limit = 127;

    if(++m_depth > depth_limit)
    {
      printf("深数限界を超過した\n");

      throw syntax_parse_error();
    }


  auto  res = process_by_formula(f,it);

  --m_depth;

    if(res)
    {

        if(m_debugging)
        {
          gbstd::print(f.name());
          printf("による解析は成功 \n");
        }


      syntax_branch  br(f.name());

      br.append(res.release());

      return result(res.iterator(),syntax_branch_element(std::move(br)));
    }

  else
    if(m_debugging)
    {
      gbstd::print(f.name());
      printf("による解析は失敗\n");
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_by_reference(std::u16string_view  name, syntax_token_string::iterator  it)
{
  auto  f = m_rule->find(name);

    if(f)
    {
      return process_by_named_formula(*f,it);
    }


  return result();
}


syntax_parser::result
syntax_parser::
step(const syntax_formula&  f, syntax_token_string::iterator  it)
{
  m_depth = 0;

  m_point_stack.clear();

  auto  res = process_by_named_formula(f,it);

    if(res)
    {
        if(res.iterator() == it)
        {
          printf("ループしたので停止した\n");

          it->print();

          print_nl();

          throw syntax_parse_error();
        }


      return std::move(res);
    }


  printf("未定義の要素が出現したので停止した\n");

  it->print();

  print_nl();

  throw syntax_parse_error();
}


syntax_parser&
syntax_parser::
set_rule(const syntax_rule&  r) noexcept
{
  m_rule = &r;

  return *this;
}


syntax_branch
syntax_parser::
start(const syntax_token_string&  toks)
{
    if(!m_rule || !*m_rule)
    {
      printf("定義がひとつもない\n");

      throw syntax_parse_error();
    }


  auto&  f = m_rule->first();

  syntax_branch  br;

//m_debugging=1;
    try
    {
      syntax_token_string::iterator  it(toks);

      it.skip();

        while(*it)
        {
          auto  res = step(f,it);

            if(res)
            {
              it = res.iterator();

              it.skip();

              br.append(res.release());
            }

          else
            {
              break;
            }
        }
    }


    catch(syntax_parse_error&)
    {
        for(auto&  pt: m_point_stack)
        {
          pt.iterator()->print();

          pt.formula().print();
 
          printf("\n");
        }


      throw;
    }


  return std::move(br);
}




}




