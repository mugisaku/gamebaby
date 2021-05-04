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
        if(res){printf("可欠要素が見付かった\n");}
      else     {printf("可欠要素は見付からなかった\n");}
    }


  return res? std::move(res)
            : result(it)
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

    if(res)
    {
        for(;;)
        {
          it = res.iterator();

          auto  cores = process_by_expression(o.get_expression(),it);

            if(!cores)
            {
              return result(it,res.release());
            }


          res = combine(std::move(res),std::move(cores));
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
process_by_operand(const syntax_operand&  o, syntax_token_iterator  it)
{
  it.skip();

  auto&  tok = *it;

       if(o.is_keyword()            ){return process_keyword(o,it);}
  else if(o.is_string()             ){return process_string(o,it);}
  else if(o.is_expression()         ){return process_by_expression(o.get_expression(),it);}
  else if(o.is_optional_expression()){return process_optional(o,it);}
  else if(o.is_multiple_expression()){return process_multiple(o,it);}
  else if(o.is_definition()         ){return process_by_definition(o.get_definition(),it);}
  else if(o.is_integer_literal()  && it->is_integer()   ){return result(++it,syntax_branch_element(tok,o));}
  else if(o.is_floating_literal() && it->is_floating()  ){return result(++it,syntax_branch_element(tok,o));}
  else if(o.is_string_literal()   && it->is_string()    ){return result(++it,syntax_branch_element(tok,o));}
  else if(o.is_identifier()       && it->is_identifier()){return result(++it,syntax_branch_element(tok,o));}


  return result();
}


}




