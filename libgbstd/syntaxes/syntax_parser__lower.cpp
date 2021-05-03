#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_parser::result
syntax_parser::
process_keyword(const syntax_operand&  o, syntax_token_iterator  it)
{
  auto  s = o.get_string();

    if(it->is_identifier() && (it->get_string() == s))
    {
      auto&  tok = *it++;

        if(m_debugging)
        {
          printf("<");
          gbstd::print(s);
          printf(">に一致\n");
        }


      return result(it,syntax_branch_element(tok,o));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_string(const syntax_operand&  o, syntax_token_iterator  it)
{
  auto  s = o.get_string();

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


      return result(it,syntax_branch_element(tok,o));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_optional(const syntax_operand&  o, syntax_token_iterator  it)
{
    if(m_debugging)
    {
      printf("可欠要素を判定\n");
    }


  auto  res = process_by_expression(o.get_expression(),it);

    if(m_debugging)
    {
        if(res.first){printf("可欠要素が見付かった\n");}
      else           {printf("可欠要素は見付からなかった\n");}
    }


  return res.first? std::move(res)
                  : result(it,syntax_branch())
  ;
}


syntax_parser::result
syntax_parser::
process_multiple(const syntax_operand&  o, syntax_token_iterator  it)
{
    if(m_debugging)
    {
      printf("複数要素を判定\n");
    }


  auto  res = process_by_expression(o.get_expression(),it);

    if(res.first)
    {
        for(;;)
        {
          auto  lbr = std::move(res.second);

          it = res.first;

          res = process_by_expression(o.get_expression(),it);

            if(!res.first)
            {
                if(m_debugging)
                {
                  printf("複数要素が%3d個見付かった\n",lbr.length());
                }


              return result(it,std::move(lbr));
            }


          lbr.splice(std::move(res.second));

          res.second = std::move(lbr);
        }
    }


    if(m_debugging)
    {
      printf("複数要素は見付からなかった\n");
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_expression(const syntax_operand&  o, syntax_token_iterator  it)
{
  auto  res = process_by_expression(o.get_expression(),it);

    if(res.first)
    {
      return std::move(res);
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_definition(const syntax_operand&  o, syntax_token_iterator  it)
{
  auto  res = process_by_definition(o.get_definition(),it);

    if(res.first)
    {
      return std::move(res);
    }


  return result();
}




syntax_parser::result
syntax_parser::
process_by_operand(const syntax_operand&  o, syntax_token_iterator  it)
{
  it.skip();

  auto&  tok = *it;

       if(o.is_keyword()            ){return process_keyword(o,it);}
  else if(o.is_string()             ){return process_string(o,it);}
  else if(o.is_expression()         ){return process_expression(o,it);}
  else if(o.is_optional_expression()){return process_optional(o,it);}
  else if(o.is_multiple_expression()){return process_multiple(o,it);}
  else if(o.is_definition()         ){return process_definition(o,it);}
  else if(o.is_integer_literal()  && it->is_integer()   ){return result(++it,syntax_branch_element(tok,o));}
  else if(o.is_floating_literal() && it->is_floating()  ){return result(++it,syntax_branch_element(tok,o));}
  else if(o.is_string_literal()   && it->is_string()    ){return result(++it,syntax_branch_element(tok,o));}
  else if(o.is_identifier()       && it->is_identifier()){return result(++it,syntax_branch_element(tok,o));}


  return result();
}


}




