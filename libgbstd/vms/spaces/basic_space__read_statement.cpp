#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;




statement
basic_space::
read_return(token_iterator&  it)
{
  auto  e = read_expression(it,";");

  ++it;

  return statement(return_statement(std::move(e)));
}


statement
basic_space::
read_jump(token_iterator&  it)
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
read_label(token_iterator&  it)
{
   if(it->is_identifier())
   {
     return statement(label_statement(it++->get_string()));
   }

 else
   {
     report;
   }


  return statement();
}


statement
basic_space::
read_if_string(token_iterator&  it)
{
  std::vector<if_statement>  ls;

  ls.emplace_back(read_if(it));

    for(;;)
    {
        if(it->is_keyword("else"))
        {
          ++it;

            if(it->is_keyword("if"))
            {
              ls.emplace_back(read_if(++it));
            }

          else
            if(it->is_operator_code("{"))
            {
                if(m_node.is_block_space())
                {
                  auto&  bsp = m_node.get_block_space().create_block_space();

                  bsp.read(++it,"}");

                  return statement(if_string_statement(std::move(ls),bsp));
                }
            }

          else
            {
              throw compile_error(it->get_line_number(),"");
            }
        }

      else
        {
          return statement(if_string_statement(std::move(ls)));
        }
    }


  throw compile_error(it->get_line_number(),"if_string error");
}


if_statement
basic_space::
read_if(token_iterator&  it)
{
    if(it->is_operator_code("("))
    {
      auto  e = read_expression(++it,")");

      ++it;

        if(it->is_operator_code("{") && m_node.is_block_space())
        {
          auto&  bsp = m_node.get_block_space().create_block_space();

          bsp.read(++it,"}");

          return if_statement(std::move(e),bsp);
        }

      else
        {
          report;
        }
    }

  else
    {
      report;
    }


  throw compile_error(it->get_line_number(),"if error");
}


statement
basic_space::
read_for(token_iterator&  it)
{
    if(it->is_operator_code("("))
    {
      auto  init = read_expression(++it,";");
      auto  cond = read_expression(++it,";");
      auto  loop = read_expression(++it,")");

      ++it;

        if(it->is_operator_code("{") && m_node.is_block_space())
        {
          auto&  bsp = m_node.get_block_space().create_block_space();

          bsp.read(++it,"}");

          return statement(for_statement(std::move(init),std::move(cond),std::move(loop),bsp));
        }
    }

  else
    {
      report;
    }


  return statement();
}


statement
basic_space::
read_while(token_iterator&  it)
{
    if(it->is_operator_code("("))
    {
      auto  e = read_expression(++it,")");

      ++it;

        if(it->is_operator_code("{") && m_node.is_block_space())
        {
          auto&  bsp = m_node.get_block_space().create_block_space();

          bsp.read(++it,"}");

          return statement(while_statement(std::move(e),bsp));
        }
    }

  else
    {
      report;
    }


  return statement();
}


statement
basic_space::
read_switch(token_iterator&  it)
{
    if(it->is_operator_code("("))
    {
      auto  e = read_expression(++it,")");

      ++it;

        if(it->is_operator_code("{") && m_node.is_block_space())
        {
          auto&  bsp = m_node.get_block_space().create_block_space();

          bsp.read(++it,"}");

          return statement(switch_statement(std::move(e),bsp));
        }
    }

  else
    {
      report;
    }


  return statement();
}


statement
basic_space::
read_case(token_iterator&  it)
{
  auto  e = read_expression(it,":");

  ++it;

  return statement(case_statement(std::move(e)));
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

          auto  scp = m_node.is_global_space()? variable_scopes::global
                     :                          variable_scopes::local
                     ;

          push_variable_info(scp,ti,name);

            if(it->is_operator_code("("))
            {
              e = read_expression(++it,")");

              ++it;
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
    if(keyword == std::string_view("label"))
    {
      push_statement(read_label(++it));
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
      push_statement(read_if_string(++it));
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
    if(keyword == std::string_view("case"))
    {
      push_statement(read_case(++it));
    }

  else
    if(keyword == std::string_view("let"))
    {
      push_statement(read_let(++it));
    }

  else
    {
      auto  e = read_expression(it,";");

      ++it;

      push_statement(expression_statement(std::move(e)));
    }
}




}




