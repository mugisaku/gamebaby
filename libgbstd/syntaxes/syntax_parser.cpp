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


namespace{
bool
test(std::u16string_view  sv, code_text::iterator  it) noexcept
{
    for(auto  c1: sv)
    {
      auto  c2 = *it++;

        if(!c2 || (c1 != c2))
        {
          return false;
        }
    }


  return true;
}
}




syntax_parser::result
syntax_parser::
process_keyword(const syntax_expression_element&  e, code_text::iterator  it)
{
  auto  s = e.get_string();

    if(test(s,it))
    {
      storage  nodes;

      it += s.size();

        if(is_alphabet(*it) || (*it == '_'))
        {
          return result();
        }


        if(1)
        {
          printf("<");
          print(s);
          printf(">に一致\n");
        }


      nodes.emplace_back(std::make_unique<syntax_tree_node>(it,e));

      return result(it,std::move(nodes));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_string(const syntax_expression_element&  e, code_text::iterator  it)
{
  auto  s = e.get_string();

    if(test(s,it))
    {
      storage  nodes;

      it += s.size();

        if(1)
        {
          printf("\"");
          print(s);
          printf("\"に一致\n");
        }


      nodes.emplace_back(std::make_unique<syntax_tree_node>(it,e));

      return result(it,std::move(nodes));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_optional(const syntax_expression_element&  e, code_text::iterator  it)
{
  auto  res = process_by_expression(e.get_expression(),it);

    if(res.first)
    {
      return std::move(res);
    }

  else
    {
      return result(it,storage());
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_multiple(const syntax_expression_element&  e, code_text::iterator  it)
{
  auto  res = process_by_expression(e.get_expression(),it);

    if(res.first)
    {
      storage  nodes;

        for(;;)
        {
          it = res.first;

          transfer(std::move(res.second),nodes);

          res = process_by_expression(e.get_expression(),it);

            if(!res.first)
            {
              return result(it,std::move(nodes));
            }
        }
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_expression(const syntax_expression_element&  e, code_text::iterator  it)
{
  auto  res = process_by_expression(e.get_expression(),it);

    if(res.first)
    {
      return std::move(res);
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_definition(const syntax_expression_element&  e, code_text::iterator  it)
{
  auto  res = process_by_definition(e.get_definition(),it);

    if(res.first)
    {
      storage  nodes;

      nodes.emplace_back(std::move(res.second));

      return result(res.first,std::move(nodes));
    }


  return result();
}




syntax_parser::result
syntax_parser::
process_by_expression_element(const syntax_expression_element&  e, code_text::iterator  it)
{
       if(e.is_keyword()            ){return process_keyword(e,it);}
  else if(e.is_string()             ){return process_string(e,it);}
  else if(e.is_expression()         ){return process_expression(e,it);}
  else if(e.is_optional_expression()){return process_optional(e,it);}
  else if(e.is_multiple_expression()){return process_multiple(e,it);}
  else if(e.is_definition()         ){return process_definition(e,it);}

  return result();
}


syntax_parser::result
syntax_parser::
process_or(const syntax_expression_element&  l, const syntax_expression_element&  r, code_text::iterator  it)
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
process_and(const syntax_expression_element&  l, const syntax_expression_element&  r, code_text::iterator  it)
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
process_colon(const syntax_expression&  expr, code_text::iterator  it)
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
process_by_expression(const syntax_expression&  expr, code_text::iterator  it)
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


std::pair<code_text::iterator,syntax_parser::node_ptr>
syntax_parser::
process_by_definition(const syntax_definition&  def, code_text::iterator  it)
{
    for(auto  defptr: m_def_stack)
    {
        if(&def == defptr)
        {
          print(def.get_name());
          printf("の循環定義が発生\n");

          return std::make_pair<code_text::iterator,node_ptr>(nullptr,nullptr);
        }
    }


    if(1)
    {
      print(def.get_name());
      printf("による解析を開始\n");
    }


  m_def_stack.emplace_back(&def);

  auto  res = process_by_expression(def,it);

  m_def_stack.pop_back();

    if(res.first)
    {
        if(1)
        {
          print(def.get_name());
          printf("による解析は成功 \n");
        }


      auto  nd = std::make_unique<syntax_tree_node>(it,def);

        for(auto&  ptr: res.second)
        {
          nd->add_child(*ptr.release());
        }


      return std::make_pair(res.first,std::move(nd));
    }

  else
    if(1)
    {
      print(def.get_name());
      printf("による解析は失敗\n");
    }


  return std::make_pair<code_text::iterator,node_ptr>(nullptr,nullptr);
}


void
syntax_parser::
process_top(const syntax_definition&  def)
{
  auto  it = m_text.get_iterator();

    while(*it)
    {
      auto  res = process_by_definition(def,it);

        if(res.first)
        {
              if(res.first == it)
              {
                printf("ループしたので停止した\n");

                it.print();

                print_nl();

                break;
              }


          it = res.first;

          m_root.add_child(*res.second.release());
        }

      else
        {
          printf("未定義の要素が出現したので停止した\n");

          it.print();

          print_nl();

          m_root.destroy_children();

          break;
        }
    }
}


void
syntax_parser::
start(std::u16string_view  def_name)
{
  auto  def = m_rule.find(def_name);

    if(!def)
    {
      printf("定義が見付からない\n");

      print(def_name);

      return;
    }


  m_root.destroy_children();

  m_def_stack.clear();

  m_point_stack.clear();

    try
    {
      process_top(*def);
    }


    catch(syntax_parse_error&)
    {
      

      throw;
    }


  m_root.print();
}




}




