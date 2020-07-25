#include"libgbstd/vms/ast.hpp"




namespace gbstd{




std::unique_ptr<ast_node>
ast_node::
read_let(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("let");

  auto  name_nd = read_name(++it);

    if(!name_nd)
    {
      throw ast_error("name for let statement is missing",*it);
    }


  nd->append_child(std::move(name_nd));

    if(it->is_operator_code("="))
    {
      ++it;
    }


  auto  type_nd = read_type(it);

    if(!type_nd)
    {
      throw ast_error("type for let statement is missing",*it);
    }


  nd->append_child(std::move(type_nd));

    if(it->is_operator_code("("))
    {
      auto  expr_nd = read_expression(++it);

        if(expr_nd)
        {
          nd->append_child(std::move(expr_nd));
        }


        if(!it->is_operator_code(")"))
        {
          throw ast_error("close operator is missing",*it);
        }


      ++it;
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_node::
read_return(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("return");

  auto  expr_nd = read_expression(++it);

    if(expr_nd)
    {
      nd->append_child(std::move(expr_nd));
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_node::
read_goto(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("goto");

  auto  lb_nd = read_name(++it);

    if(lb_nd)
    {
      nd->append_child(std::move(lb_nd));

      return std::move(nd);
    }


  throw ast_error("label for goto is missing",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_case(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("case");

  auto  expr_nd = read_expression(++it);

    if(expr_nd && it->is_operator_code(":"))
    {
      ++it;

      nd->append_child(std::move(expr_nd));

      return std::move(nd);
    }


  throw ast_error("expression for case is missing",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_switch(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("switch");

  ++it;

    if(it->is_operator_code("("))
    {
      auto  expr_nd = read_expression(++it);

        if(expr_nd && it->is_operator_code(")"))
        {
          ++it;

            if(it->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space(it);

                if(sp_nd)
                {
                  nd->append_child(std::move(expr_nd));
                  nd->append_child(std::move(  sp_nd));

                  return std::move(nd);
                }
            }
        }


      throw ast_error("switch body is missing",*it);
    }


  throw ast_error("expression for switch is missing",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_for_instruction(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("for instruction");

  auto  init_nd = read_let(++it);

    if(init_nd)
    {
      nd->append_child(std::move(init_nd),"init");
    }


    if(it->is_operator_code(";"))
    {
      ++it;
    }


  auto  cond_nd = read_expression(it);

    if(cond_nd)
    {
      nd->append_child(std::move(cond_nd),"cond");
    }


    if(it->is_operator_code(";"))
    {
      ++it;
    }


  auto  loop_nd = read_expression(it);

    if(loop_nd)
    {
      nd->append_child(std::move(loop_nd),"loop");
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_node::
read_for(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("for");

  ++it;

    if(!it->is_operator_code("("))
    {
      throw ast_error("for instruction is missing",*it);
    }


  auto  instr_nd = read_for_instruction(it);

    if(it->is_operator_code(")"))
    {
      ++it;
    }


    if(!it->is_operator_code("{"))
    {
      throw ast_error("for body is missing",*it);
    }


  auto  sp_nd = read_local_space(++it);

  nd->append_child(std::move(instr_nd));
  nd->append_child(std::move(   sp_nd));

  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_node::
read_while(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("while");

  ++it;

    if(it->is_operator_code("("))
    {
      auto  expr_nd = read_expression(++it);

        if(expr_nd && it->is_operator_code(")"))
        {
          ++it;

            if(it->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space(it);

                if(sp_nd)
                {
                  nd->append_child(std::move(expr_nd));
                  nd->append_child(std::move(  sp_nd));

                  return std::move(nd);
                }
            }
        }
    }


  throw ast_error("expression for while is missing",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_if(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("if");

  ++it;

    if(it->is_operator_code("("))
    {
      auto  expr_nd = read_expression(++it);

        if(expr_nd && it->is_operator_code(")"))
        {
          ++it;

            if(it->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space(it);

                if(sp_nd)
                {
                  nd->append_child(std::move(expr_nd));
                  nd->append_child(std::move(  sp_nd));

                  return std::move(nd);
                }
            }


          throw ast_error("if body is missing",*it);
        }
    }


  throw ast_error("expression for if is missing",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_if_string(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("if string");

  auto  if_nd = read_if(it);

    if(if_nd)
    {
      nd->append_child(std::move(if_nd));

        while(it->is_keyword("else"))
        {
          ++it;

            if(it->is_keyword("if"))
            {
              if_nd = read_if(it);

                if(if_nd)
                {
                  nd->append_child(std::move(if_nd));
                }
            }

          else
            if(it->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space(it);

                if(sp_nd)
                {
                  nd->append_child(std::move(sp_nd));

                  break;
                }
            }

          else
            {
              throw ast_error("else body is missing",*it);
            }
        }
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_node::
read_statement(token_iterator&  it)
{
    if(it->is_identifier())
    {
      auto&  s = it->get_string();

        if(s == std::string_view("return"))
        {
          return read_return(it);
        }

      else
        if(s == std::string_view("if"))
        {
          return read_if_string(it);
        }

      else
        if(s == std::string_view("for"))
        {
          return read_for(it);
        }

      else
        if(s == std::string_view("while"))
        {
          return read_while(it);
        }

      else
        if(s == std::string_view("switch"))
        {
          return read_switch(it);
        }

      else
        if(s == std::string_view("case"))
        {
          return read_case(it);
        }

      else
        if(s == std::string_view("break"))
        {
          return std::make_unique<ast_node>(*it++,"break");
        }

      else
        if(s == std::string_view("continue"))
        {
          return std::make_unique<ast_node>(*it++,"continue");
        }

      else
        if(s == std::string_view("goto"))
        {
          return read_goto(it);
        }

      else
        if(s == std::string_view("let"))
        {
          return read_let(it);
        }

      else
        if(it[1].is_operator_code(":"))
        {
          auto  lb_nd = std::make_unique<ast_node>(*it,"label");

          it += 2;

          return std::move(lb_nd);
        }
    }


  return read_expression(it);
}




}




