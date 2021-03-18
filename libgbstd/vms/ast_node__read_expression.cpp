#include"libgbstd/vms/ast.hpp"




namespace gbstd{




std::unique_ptr<ast_node>
ast_builder::
read_operand()
{
    if(m_iterator->is_integer() ||
       m_iterator->is_floating_point_number() ||
       m_iterator->is_string() ||
       m_iterator->is_identifier())
    {
      return make_ast_node(*m_iterator++,"operand");
    }

  else
    if(m_iterator->is_operator_code("("))
    {
      auto       nd = make_ast_node(*m_iterator++,"operand");
      auto  expr_nd = read_expression();

        if(expr_nd && m_iterator->is_operator_code(")"))
        {
          advance();

          nd->append_child(std::move(expr_nd));

          return std::move(nd);
        }
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_argument_list()
{
  auto  nd = std::make_unique<ast_node>("argument list");

  advance();

    while(m_iterator)
    {
      ast_loopguard  lg("read_argument_list",m_iterator);

        if(m_iterator->is_operator_code(")"))
        {
          advance();

          return std::move(nd);
        }


      auto  expr_nd = read_expression();

        if(expr_nd)
        {
          nd->append_child(std::move(expr_nd));

            if(m_iterator->is_operator_code(","))
            {
              advance();
            }
        }

      else
        {
          break;
        }


      lg(m_iterator);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_index()
{
  auto  nd = std::make_unique<ast_node>("index");

  advance();

  auto  expr_nd = read_expression();

    if(expr_nd && m_iterator->is_operator_code("]"))
    {
      advance();

      nd->append_child(std::move(expr_nd));

      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_primary_expression_element()
{
  constexpr auto  id = std::string_view("primary expression element");

    if(m_iterator->is_operator_code(".") ||
       m_iterator->is_operator_code("->"))
    {
      auto  nd = make_ast_node(*m_iterator++,id);

        if(m_iterator->is_identifier())
        {
          nd->append_child(make_ast_node(*m_iterator++,"identifier"));

          return std::move(nd);
        }
    }

  else
    if(m_iterator->is_operator_code("["))
    {
      auto  nd = make_ast_node(*m_iterator,id);

      auto  idx_nd = read_index();

        if(idx_nd)
        {
          nd->append_child(std::move(idx_nd));

          return std::move(nd);
        }
    }

  else
    if(m_iterator->is_operator_code("("))
    {
      auto  nd = make_ast_node(*m_iterator,id);

      auto  args_nd = read_argument_list();

        if(args_nd)
        {
          nd->append_child(std::move(args_nd));

          return std::move(nd);
        }
    }

  else
    if(m_iterator->is_operator_code("++") ||
       m_iterator->is_operator_code("--"))
    {
      return make_ast_node(*m_iterator++,id);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_primary_expression()
{
  auto  nd = std::make_unique<ast_node>("primary expression");

  auto  o_nd = read_operand();

    if(o_nd)
    {
      nd->append_child(std::move(o_nd));

        for(;;)
        {
          ast_loopguard  lg("read_primary_expression",m_iterator);

          auto  prexpre = read_primary_expression_element();

            if(!prexpre)
            {
              break;
            }


          nd->append_child(std::move(prexpre));

          lg(m_iterator);
        }


      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_unary_operator()
{
  auto  opco = m_iterator->get_operator_code();

    if((opco == "++") ||
       (opco == "--") ||
       (opco == "-") ||
       (opco == "*") ||
       (opco == "&") ||
       (opco == "!") ||
       (opco == "~"))
    {
      return make_ast_node(*m_iterator++,"unary operator");
    } 


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_unary_expression()
{
  auto  nd = std::make_unique<ast_node>("unary expression");

    for(;;)
    {
      ast_loopguard  lg("read_unary_expression",m_iterator);

      auto  unop_nd = read_unary_operator();

        if(!unop_nd)
        {
          break;
        }


      nd->append_child(std::move(unop_nd));

      lg(m_iterator);
    }


  auto  prexpr = read_primary_expression();

    if(prexpr)
    {
      nd->append_child(std::move(prexpr));

      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_binary_operator()
{
  auto  opco = m_iterator->get_operator_code();

    if((opco == "+") ||
       (opco == "-") ||
       (opco == "*") ||
       (opco == "/") ||
       (opco == "%") ||
       (opco == "<<") ||
       (opco == ">>") ||
       (opco == "|") ||
       (opco == "&") ||
       (opco == "^") ||
       (opco == "==") ||
       (opco == "!=") ||
       (opco == "<") ||
       (opco == "<=") ||
       (opco == ">") ||
       (opco == ">=") ||
       (opco == "||") ||
       (opco == "&&") ||
       (opco == "=") ||
       (opco == "+=") ||
       (opco == "-=") ||
       (opco == "*=") ||
       (opco == "/=") ||
       (opco == "%=") ||
       (opco == "<<=") ||
       (opco == ">>=") ||
       (opco == "|=") ||
       (opco == "&=") ||
       (opco == "^="))
    {
      return make_ast_node(*m_iterator++,"binary operator");
    } 


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_expression_element()
{
  auto  nd = std::make_unique<ast_node>("expression element");

  auto   binop_nd = read_binary_operator();
  auto  unexpr_nd = read_unary_expression();

    if(binop_nd && unexpr_nd)
    {
      nd->append_child(std::move( binop_nd));
      nd->append_child(std::move(unexpr_nd));

      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_expression()
{
  auto  nd = std::make_unique<ast_node>("expression");

  auto  first_expr_nd = read_unary_expression();

    if(first_expr_nd)
    {
      nd->append_child(std::move(first_expr_nd));

        for(;;)
        {
          ast_loopguard  lg("read_expression",m_iterator);

          auto  expre_nd = read_expression_element();

            if(!expre_nd)
            {
              break;
            }


          nd->append_child(std::move(expre_nd));

          lg(m_iterator);
        }


      nd->content().set_data(new ast_constant(0));

      return std::move(nd);
    }


  return nullptr;
}




}




