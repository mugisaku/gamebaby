#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_parser::result
syntax_parser::
combine(result&&  l, result&&  r) noexcept
{
  l.branch().splice(r.release());

  return result(r.iterator(),l.release());
}




syntax_parser::result
syntax_parser::
process_or(const syntax_operand&  l, const syntax_operand&  r, syntax_token_iterator  it)
{
    if(m_debugging)
    {
      printf("or判定を開始\n");
    }


  auto  res = process_by_operand(l,it);

    if(res)
    {
        if(m_debugging)
        {
          printf("左辺を採用\n");
        }


      return std::move(res);
    }


  res = process_by_operand(r,it);

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
process_and(const syntax_operand&  l, const syntax_operand&  r, syntax_token_iterator  it)
{
    if(m_debugging)
    {
      printf("and判定を開始\n");
    }


  auto  lres = process_by_operand(l,it);

    if(lres)
    {
      auto  rres = process_by_operand(r,lres.iterator());

        if(rres)
        {
          return combine(std::move(lres),std::move(rres));
        }
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_colon(const syntax_expression&  expr, syntax_token_iterator  it)
{
    if(m_debugging)
    {
      printf("colon判定を開始\n");
    }


  auto  lres = process_by_operand(*expr.left(),it);

    if(lres)
    {
      m_point_stack.emplace_back(it,expr);

      auto  rres = process_by_operand(*expr.right(),lres.iterator());

        if(rres)
        {
          return combine(std::move(lres),std::move(rres));
        }


//      printf(":後の解析失敗");

//      it->print();

//      throw syntax_parse_error();
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_by_expression(const syntax_expression&  expr, syntax_token_iterator  it)
{
  auto  code = expr.code();
  auto&  l = expr.left();
  auto&  r = expr.right();

       if(code == '|'){return process_or( *l,*r,it);}
  else if(code == '&'){return process_and(*l,*r,it);}
  else if(code == ':'){return process_colon(expr,it);}
  else
    if(l)
    {
      auto  res = process_by_operand(*l,it);

        if(res)
        {
          return std::move(res);
        }
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_by_definition(const syntax_definition&  def, syntax_token_iterator  it)
{
    if(m_debugging)
    {
      gbstd::print(def.get_name());
      printf("による解析を開始\n");
    }


  constexpr int  depth_limit = 127;

    if(++m_depth > depth_limit)
    {
      printf("深数限界を超過した\n");

      throw syntax_parse_error();
    }


  auto  res = process_by_expression(def,it);

  --m_depth;

    if(res)
    {
        if(m_debugging)
        {
          gbstd::print(def.get_name());
          printf("による解析は成功 \n");
        }


      syntax_branch_element  e(*it,def,res.release());

      return result(res.iterator(),syntax_branch(std::move(e)));
    }

  else
    if(m_debugging)
    {
      gbstd::print(def.get_name());
      printf("による解析は失敗\n");
    }


  return result();
}


syntax_parser::result
syntax_parser::
step(const syntax_definition&  def, syntax_token_iterator  it)
{
  m_depth = 0;

  m_point_stack.clear();

  auto  res = process_by_definition(def,it);

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
start(const syntax_branch_source&  src)
{
    if(!m_rule || !*m_rule)
    {
      printf("定義がひとつもない\n");

      throw syntax_parse_error();
    }


  auto&  def = m_rule->first();

  syntax_branch  br;

//m_debugging=1;
    try
    {
      auto  it = src.iterator();

      it.skip();

        while(*it)
        {
          auto  res = step(def,it);

            if(res)
            {
              it = res.iterator();

              it.skip();

              br.splice(res.release());
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

          pt.expression().print();
 
          printf("\n");
        }


      throw;
    }


  return std::move(br);
}




}




