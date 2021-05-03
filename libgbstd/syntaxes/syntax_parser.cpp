#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_parser::result
syntax_parser::
process_or(const syntax_operand&  l, const syntax_operand&  r, syntax_token_iterator  it)
{
    if(m_debugging)
    {
      printf("or判定を開始\n");
    }


  auto  res = process_by_operand(l,it);

    if(res.first)
    {
        if(m_debugging)
        {
          printf("左辺を採用\n");
        }


      return std::move(res);
    }


  res = process_by_operand(r,it);

    if(res.first)
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


  auto  res = process_by_operand(l,it);

    if(res.first)
    {
      auto  lbr = std::move(res.second);

      res = process_by_operand(r,res.first);

        if(res.first)
        {
          lbr.splice(std::move(res.second));

          res.second = std::move(lbr);

          return result(res);
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


  auto  res = process_by_operand(*expr.left(),it);

    if(res.first)
    {
      auto  lbr = std::move(res.second);

      m_point_stack.emplace_back(it,expr);

      res = process_by_operand(*expr.right(),res.first);

        if(res.first)
        {
          lbr.splice(std::move(res.second));

          res.second = std::move(lbr);

          return result(res);
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

        if(res.first)
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

    if(res.first)
    {
        if(m_debugging)
        {
          gbstd::print(def.get_name());
          printf("による解析は成功 \n");
        }


    }

  else
    if(m_debugging)
    {
      gbstd::print(def.get_name());
      printf("による解析は失敗\n");
    }


  return result();
}


syntax_token_iterator
syntax_parser::
step(const syntax_definition&  def, syntax_token_iterator  it)
{
  m_depth = 0;

  m_point_stack.clear();

  auto  res = process_by_definition(def,it);

    if(res.first)
    {
        if(res.first == it)
        {
          printf("ループしたので停止した\n");

          it->print();

          print_nl();

          throw syntax_parse_error();
        }


      m_branch.splice(std::move(res.second));

      return res.first;
    }


  printf("未定義の要素が出現したので停止した\n");

  it->print();

  print_nl();

  m_branch.cut_back();

  throw syntax_parse_error();
}


void
syntax_parser::
start(std::u16string_view  def_name)
{
  reset();


  auto  def = m_rule.find(def_name);

    if(!def)
    {
      printf("定義が見付からない\n");

      gbstd::print(def_name);

      throw syntax_parse_error();
    }


//m_debugging=1;
    try
    {
      syntax_token_iterator  it(m_token_string);

      it.skip();

        while(*it)
        {
          it = step(*def,it);

          it.skip();
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


  m_branch.print();
}


void
syntax_parser::
reset() noexcept
{
  m_token_string = make_token_string(m_text.get_string());

  m_branch.cut_back();
}


void
syntax_parser::
print() const noexcept
{
    for(auto&  tok: m_token_string)
    {
      tok.print();

      print_nl();
    }
}




}




