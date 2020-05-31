#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;




statement
basic_space::
read_return(token_iterator&  it)
{
  return statement(return_statement(read_expression(it,";")));
}


statement
basic_space::
read_jump(token_iterator&  it) noexcept
{
   if(it->is_identifier())
   {
     return statement(jump_statement(it++->get_string()));
   }

 else
   {
     report;
   }


  return statement();
}


statement
basic_space::
read_label(token_iterator&  it) noexcept
{
  return statement();
}


statement
basic_space::
read_if(token_iterator&  it) noexcept
{
    if(it->is_operator_code("("))
    {
    }

  else
    {
      report;
    }


  return statement();
}


statement
basic_space::
read_for(token_iterator&  it) noexcept
{
    if(it->is_operator_code("("))
    {
    }

  else
    {
      report;
    }


  return statement();
}


statement
basic_space::
read_while(token_iterator&  it) noexcept
{
    if(it->is_operator_code("("))
    {
    }

  else
    {
      report;
    }


  return statement();
}


statement
basic_space::
read_switch(token_iterator&  it) noexcept
{
    if(it->is_operator_code("("))
    {
    }

  else
    {
      report;
    }


  return statement();
}


statement
basic_space::
read_let(token_iterator&  it)
{
  auto&  it0 = it[0];
  auto&  it1 = it[1];

    if(it0.is_identifier() && it1.is_operator_code("="))
    {
      auto&  name = it0.get_string();

      it += 2;

      auto  ti = read_derived_type_info(it);

        if(ti)
        {
          expression  e;

          push_memo_info(ti,name);

            if(it->is_operator_code("("))
            {
              e = read_expression(++it,")");
            }


          return statement(let_statement(name,std::move(e)));
        }
    }


  return statement();
}


void
basic_space::
read_statement(std::string_view  keyword, token_iterator&  it)
{
    if(keyword == std::string_view("return"))
    {
      push_statement(read_return(++it));
    }

  else
    if(keyword == std::string_view("jump"))
    {
      push_statement(read_jump(++it));
    }

  else
    if((keyword ==     "break") ||
       (keyword ==  "continue") ||
       (keyword == "interrupt"))
    {
      ++it;

      push_statement(statement(control_statement(keyword)));
    }

  else
    if(keyword == std::string_view("if"))
    {
      push_statement(read_if(++it));
    }

  else
    if(keyword == std::string_view("while"))
    {
      push_statement(read_while(++it));
    }

  else
    if(keyword == std::string_view("for"))
    {
      push_statement(read_for(++it));
    }

  else
    if(keyword == std::string_view("switch"))
    {
      push_statement(read_switch(++it));
    }

  else
    if(keyword == std::string_view("let"))
    {
      push_statement(read_let(++it));
    }

  else
    {
      push_statement(expression_statement(read_expression(it,";")));
    }
}




}




