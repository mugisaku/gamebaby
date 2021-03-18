#include"libgbstd/vms/ast.hpp"




namespace gbstd{




std::unique_ptr<ast_node>
ast_builder::
read_let()
{
  auto  nd = make_ast_node("let");

  advance();

  auto  name_nd = read_name();

    if(!name_nd)
    {
      throw ast_error("name for let statement is missing",*m_iterator);
    }


  nd->append_child(std::move(name_nd));

    if(m_iterator->is_operator_code("="))
    {
      advance();
    }


  auto  type_nd = read_type();

    if(!type_nd)
    {
      throw ast_error("type for let statement is missing",*m_iterator);
    }


  nd->append_child(std::move(type_nd));

    if(m_iterator->is_operator_code("("))
    {
      advance();

      auto  expr_nd = read_expression();

        if(expr_nd)
        {
          nd->append_child(std::move(expr_nd));
        }


        if(!m_iterator->is_operator_code(")"))
        {
          throw ast_error("close operator is missing",*m_iterator);
        }


      advance();
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_builder::
read_return()
{
  auto  nd = make_ast_node("return");

  advance();

  auto  expr_nd = read_expression();

    if(expr_nd)
    {
      nd->append_child(std::move(expr_nd));
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_builder::
read_goto()
{
  auto  nd = make_ast_node("goto");

  advance();

  auto  lb_nd = read_name();

    if(lb_nd)
    {
      nd->append_child(std::move(lb_nd));

      return std::move(nd);
    }


  throw ast_error("label for goto is missing",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_case()
{
  auto  nd = make_ast_node("case");

  advance();

  auto  expr_nd = read_expression();

    if(expr_nd && m_iterator->is_operator_code(":"))
    {
      advance();

      nd->append_child(std::move(expr_nd));

      return std::move(nd);
    }


  throw ast_error("expression for case is missing",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_switch()
{
  auto  nd = make_ast_node("switch");

  advance();

    if(m_iterator->is_operator_code("("))
    {
      advance();

      auto  expr_nd = read_expression();

        if(expr_nd && m_iterator->is_operator_code(")"))
        {
          advance();

            if(m_iterator->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space();

                if(sp_nd)
                {
                  nd->append_child(std::move(expr_nd));
                  nd->append_child(std::move(  sp_nd));

                  return std::move(nd);
                }
            }
        }


      throw ast_error("switch body is missing",*m_iterator);
    }


  throw ast_error("expression for switch is missing",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_for_instruction()
{
  auto  nd = make_ast_node("for instruction");

  advance();

  auto  init_nd = read_let();

    if(init_nd)
    {
      nd->append_child(rename(std::move(init_nd),"init"));
    }


    if(m_iterator->is_operator_code(";"))
    {
      advance();
    }


  auto  cond_nd = read_expression();

    if(cond_nd)
    {
      nd->append_child(rename(std::move(cond_nd),"cond"));
    }


    if(m_iterator->is_operator_code(";"))
    {
      advance();
    }


  auto  loop_nd = read_expression();

    if(loop_nd)
    {
      nd->append_child(rename(std::move(loop_nd),"loop"));
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_builder::
read_for()
{
  auto  nd = make_ast_node("for");

  advance();

    if(!m_iterator->is_operator_code("("))
    {
      throw ast_error("for instruction is missing",*m_iterator);
    }


  auto  instr_nd = read_for_instruction();

    if(m_iterator->is_operator_code(")"))
    {
      advance();
    }


    if(!m_iterator->is_operator_code("{"))
    {
      throw ast_error("for body is missing",*m_iterator);
    }


  advance();

  auto  sp_nd = read_local_space();

  nd->append_child(std::move(instr_nd));
  nd->append_child(std::move(   sp_nd));

  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_builder::
read_while()
{
  auto  nd = make_ast_node("while");

  advance();

    if(m_iterator->is_operator_code("("))
    {
      advance();

      auto  expr_nd = read_expression();

        if(expr_nd && m_iterator->is_operator_code(")"))
        {
          advance();

            if(m_iterator->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space();

                if(sp_nd)
                {
                  nd->append_child(std::move(expr_nd));
                  nd->append_child(std::move(  sp_nd));

                  return std::move(nd);
                }
            }
        }
    }


  throw ast_error("expression for while is missing",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_if()
{
  auto  nd = make_ast_node("if");

  advance();

    if(m_iterator->is_operator_code("("))
    {
      advance();

      auto  expr_nd = read_expression();

        if(expr_nd && m_iterator->is_operator_code(")"))
        {
          advance();

            if(m_iterator->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space();

                if(sp_nd)
                {
                  nd->append_child(std::move(expr_nd));
                  nd->append_child(std::move(  sp_nd));

                  return std::move(nd);
                }
            }


          throw ast_error("if body is missing",*m_iterator);
        }
    }


  throw ast_error("expression for if is missing",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_if_string()
{
  auto  nd = make_ast_node("if string");

  auto  if_nd = read_if();

    if(if_nd)
    {
      nd->append_child(std::move(if_nd));

        while(m_iterator->is_keyword("else"))
        {
          advance();

            if(m_iterator->is_keyword("if"))
            {
              if_nd = read_if();

                if(if_nd)
                {
                  nd->append_child(std::move(if_nd));
                }
            }

          else
            if(m_iterator->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space();

                if(sp_nd)
                {
                  nd->append_child(std::move(sp_nd));

                  break;
                }
            }

          else
            {
              throw ast_error("else body is missing",*m_iterator);
            }
        }
    }


  return std::move(nd);
}


std::unique_ptr<ast_node>
ast_builder::
read_statement()
{
    while(m_iterator->is_operator_code(";"))
    {
      advance();
    }


    if(m_iterator->is_identifier())
    {
      auto&  s = m_iterator->get_string();

           if(s == std::string_view("return")){return read_return();}
      else if(s == std::string_view("if"    )){return read_if_string();}
      else if(s == std::string_view("for"   )){return read_for();}
      else if(s == std::string_view("while" )){return read_while();}
      else if(s == std::string_view("switch")){return read_switch();}
      else if(s == std::string_view("case"  )){return read_case();}
      else if(s == std::string_view("goto"  )){return read_goto();}
      else if(s == std::string_view("let"   )){return read_let();}
      else
        if(s == std::string_view("break"))
        {
          return make_ast_node(*m_iterator++,"break");
        }

      else
        if(s == std::string_view("continue"))
        {
          return make_ast_node(*m_iterator++,"continue");
        }

      else
        if(m_iterator[1].is_operator_code(":"))
        {
          auto  lb_nd = make_ast_node(*m_iterator,"label");

          advance(2);

          return std::move(lb_nd);
        }
    }


  return read_expression();
}




}




