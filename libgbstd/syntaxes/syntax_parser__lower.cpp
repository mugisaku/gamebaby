#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_parser::result
syntax_parser::
process_keyword(const syntax_element&  e, syntax_token_string::iterator  it)
{
  auto  s = e.string();

    if(it->is_identifier() && (it->string() == s))
    {
      auto&  tok = *it++;

        if(m_debugging)
        {
          printf("<");
          gbstd::print(s);
          printf(">に一致\n");
        }


      return result(it,syntax_branch_element(tok,true));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_string(const syntax_element&  e, syntax_token_string::iterator  it)
{
  auto  s = e.string();

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


      return result(it,syntax_branch_element(tok,false));
    }


  return result();
}


syntax_parser::result
syntax_parser::
process_optional(const syntax_element&  e, syntax_token_string::iterator  it)
{
    if(m_debugging)
    {
      printf("可欠要素を判定\n");
    }


  auto  res = process_by_formula(e.formula(),it);

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
process_multiple(const syntax_element&  e, syntax_token_string::iterator  it)
{
    if(m_debugging)
    {
      printf("複数要素を判定\n");
    }


  auto  res = process_by_formula(e.formula(),it);

    if(res)
    {
        for(;;)
        {
          it = res.iterator();

          auto  next_res = process_by_formula(e.formula(),it);

            if(!next_res)
            {
              return std::move(res);
            }


          res = result(std::move(res),std::move(next_res));
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
process_by_element(const syntax_element&  e, syntax_token_string::iterator  it)
{
  it.skip();

  auto&  tok = *it;

       if(e.is_keyword()         ){return process_keyword(e,it);}
  else if(e.is_string()          ){return process_string(e,it);}
  else if(e.is_formula()         ){return process_by_formula(e.formula(),it);}
  else if(e.is_optional_formula()){return process_optional(e,it);}
  else if(e.is_multiple_formula()){return process_multiple(e,it);}
  else if(e.is_reference()       ){return process_by_reference(e.string(),it);}
  else if(e.is_integer_literal()  && it->is_integer()   ){return result(++it,syntax_branch_element(tok));}
  else if(e.is_floating_literal() && it->is_floating()  ){return result(++it,syntax_branch_element(tok));}
  else if(e.is_string_literal()   && it->is_string()    ){return result(++it,syntax_branch_element(tok));}
  else if(e.is_identifier()       && it->is_identifier()){return result(++it,syntax_branch_element(tok));}


  return result();
}


}




