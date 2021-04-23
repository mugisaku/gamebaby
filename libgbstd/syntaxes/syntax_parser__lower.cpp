#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_parser::result
syntax_parser::
process_keyword(const syntax_expression_element&  e, syntax_token_iterator  it)
{
  auto  s = e.get_string();

    if(it->is_identifier() && (it->get_string() == s))
    {
      auto&  tok = *it++;

        if(m_debugging)
        {
          printf("<");
          gbstd::print(s);
          printf(">に一致\n");
        }


      return result(it,make_storage(tok,e));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_string(const syntax_expression_element&  e, syntax_token_iterator  it)
{
  auto  s = e.get_string();

    if(it.test(s))
    {
      auto&  tok = *it;

      it += s.size();

        if(m_debugging)
        {
          printf("\"");
          gbstd::print(s);
          printf("\"に一致\n");
        }


      return result(it,make_storage(tok,e));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_optional(const syntax_expression_element&  e, syntax_token_iterator  it)
{
  auto  res = process_by_expression(e.get_expression(),it);

  return res.first? std::move(res)
                  : result(it,storage())
  ;
}


syntax_parser::result
syntax_parser::
process_multiple(const syntax_expression_element&  e, syntax_token_iterator  it)
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
process_expression(const syntax_expression_element&  e, syntax_token_iterator  it)
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
process_definition(const syntax_expression_element&  e, syntax_token_iterator  it)
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
process_by_expression_element(const syntax_expression_element&  e, syntax_token_iterator  it)
{
  it.skip();

  auto&  tok = *it;

       if(e.is_keyword()            ){return process_keyword(e,it);}
  else if(e.is_string()             ){return process_string(e,it);}
  else if(e.is_expression()         ){return process_expression(e,it);}
  else if(e.is_optional_expression()){return process_optional(e,it);}
  else if(e.is_multiple_expression()){return process_multiple(e,it);}
  else if(e.is_definition()         ){return process_definition(e,it);}
  else if(e.is_integer_literal()  && it->is_integer()   ){return result(++it,make_storage(tok,e));}
  else if(e.is_floating_literal() && it->is_floating()  ){return result(++it,make_storage(tok,e));}
  else if(e.is_string_literal()   && it->is_string()    ){return result(++it,make_storage(tok,e));}
  else if(e.is_identifier()       && it->is_identifier()){return result(++it,make_storage(tok,e));}

  return result();
}


}




