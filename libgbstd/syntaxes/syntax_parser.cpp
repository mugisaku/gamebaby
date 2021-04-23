#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_parser::
transfer(storage&&  src, storage&  dst) noexcept
{
    for(auto&  ptr: src)
    {
      dst.emplace_back(std::move(ptr));
    }
}




syntax_parser::result
syntax_parser::
process_or(const syntax_expression_element&  l, const syntax_expression_element&  r, syntax_token_iterator  it)
{
  storage  nodes;

  auto  res = process_by_expression_element(l,it);

    if(res.first)
    {
      return std::move(res);
    }


  res = process_by_expression_element(r,it);

    if(res.first)
    {
      return std::move(res);
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_and(const syntax_expression_element&  l, const syntax_expression_element&  r, syntax_token_iterator  it)
{
  storage  nodes;

  auto  res = process_by_expression_element(l,it);

    if(res.first)
    {
      transfer(std::move(res.second),nodes);

      res = process_by_expression_element(r,res.first);

        if(res.first)
        {
          transfer(std::move(res.second),nodes);

          return result(res.first,std::move(nodes));
        }
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_colon(const syntax_expression&  expr, syntax_token_iterator  it)
{
  storage  nodes;

  auto  res = process_by_expression_element(*expr.left(),it);

    if(res.first)
    {
      m_point_stack.emplace_back(it,expr);

      transfer(std::move(res.second),nodes);

      res = process_by_expression_element(*expr.right(),res.first);

        if(res.first)
        {
          transfer(std::move(res.second),nodes);

          return result(res.first,std::move(nodes));
        }


      throw syntax_parse_error();
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_by_expression(const syntax_expression&  expr, syntax_token_iterator  it)
{
  storage  nodes;

  auto  code = expr.code();
  auto&  l = expr.left();
  auto&  r = expr.right();

       if(code == '|'){return process_or( *l,*r,it);}
  else if(code == '&'){return process_and(*l,*r,it);}
  else if(code == ':'){return process_colon(expr,it);}
  else
    if(l)
    {
      auto  res = process_by_expression_element(*l,it);

        if(res.first)
        {
          return std::move(res);
        }
    }


  return result();
}


std::pair<syntax_token_iterator,syntax_parser::node_ptr>
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


      auto  nd = std::make_unique<syntax_tree_node>(*it,def);

        for(auto&  ptr: res.second)
        {
          nd->add_child(*ptr.release());
        }


      return std::make_pair(res.first,std::move(nd));
    }

  else
    if(m_debugging)
    {
      gbstd::print(def.get_name());
      printf("による解析は失敗\n");
    }


  return std::make_pair<syntax_token_iterator,node_ptr>({},nullptr);
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


      m_root.add_child(*res.second.release());

      return res.first;
    }


  printf("未定義の要素が出現したので停止した\n");

  it->print();

  print_nl();

  m_root.destroy_children();

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
/*
        for(auto&  pt: m_point_stack)
        {
          pt.iterator()->print();

          pt.expression().print();
 
          printf("\n");
        }
*/

      throw;
    }


  m_root.print();
}


void
syntax_parser::
reset() noexcept
{
  m_token_string = make_token_string(m_text.get_string());

  m_root.destroy_children();
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




