#include"libgbstd/vms/ast.hpp"




namespace gbstd{




std::unique_ptr<ast_node>
ast_node::
read_operand(token_iterator&  it)
{
    if(it->is_integer() ||
       it->is_floating_point_number() ||
       it->is_string() ||
       it->is_identifier())
    {
      return std::make_unique<ast_node>(*it++,"operand");
    }

  else
    if(it->is_operator_code("("))
    {
      auto       nd = std::make_unique<ast_node>(*it++,"operand");
      auto  expr_nd = read_expression(it);

        if(expr_nd && it->is_operator_code(")"))
        {
          ++it;

          nd->append_child(std::move(expr_nd));

          return std::move(nd);
        }
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_argument_list(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("argument list");

  ++it;

    while(it)
    {
      ast_loopguard  lg("read_argument_list",it);

        if(it->is_operator_code(")"))
        {
          ++it;

          return std::move(nd);
        }


      auto  expr_nd = read_expression(it);

        if(expr_nd)
        {
          nd->append_child(std::move(expr_nd));

            if(it->is_operator_code(","))
            {
              ++it;
            }
        }

      else
        {
          break;
        }


      lg(it);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_index(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("index");

  auto  expr_nd = read_expression(++it);

    if(expr_nd && it->is_operator_code("]"))
    {
      ++it;

      nd->append_child(std::move(expr_nd));

      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_primary_expression_element(token_iterator&  it)
{
  constexpr auto  id = std::string_view("primary expression element");

    if(it->is_operator_code(".") ||
       it->is_operator_code("->"))
    {
      auto  nd = std::make_unique<ast_node>(*it++,id);

        if(it->is_identifier())
        {
          nd->append_child(std::make_unique<ast_node>(*it++,"identifier"));

          return std::move(nd);
        }
    }

  else
    if(it->is_operator_code("["))
    {
      auto  nd = std::make_unique<ast_node>(*it,id);

      auto  idx_nd = read_index(it);

        if(idx_nd)
        {
          nd->append_child(std::move(idx_nd));

          return std::move(nd);
        }
    }

  else
    if(it->is_operator_code("("))
    {
      auto  nd = std::make_unique<ast_node>(*it,id);

      auto  args_nd = read_argument_list(it);

        if(args_nd)
        {
          nd->append_child(std::move(args_nd));

          return std::move(nd);
        }
    }

  else
    if(it->is_operator_code("++") ||
       it->is_operator_code("--"))
    {
      return std::make_unique<ast_node>(*it++,id);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_primary_expression(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("primary expression");

  auto  o_nd = read_operand(it);

    if(o_nd)
    {
      nd->append_child(std::move(o_nd));

        for(;;)
        {
          ast_loopguard  lg("read_primary_expression",it);

          auto  prexpre = read_primary_expression_element(it);

            if(!prexpre)
            {
              break;
            }


          nd->append_child(std::move(prexpre));

          lg(it);
        }


      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_unary_operator(token_iterator&  it)
{
  auto  opco = it->get_operator_code();

    if((opco == "++") ||
       (opco == "--") ||
       (opco == "-") ||
       (opco == "*") ||
       (opco == "&") ||
       (opco == "!") ||
       (opco == "~"))
    {
      return std::make_unique<ast_node>(*it++,"unary operator");
    } 


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_unary_expression(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("unary expression");

    for(;;)
    {
      ast_loopguard  lg("read_unary_expression",it);

      auto  unop_nd = read_unary_operator(it);

        if(!unop_nd)
        {
          break;
        }


      nd->append_child(std::move(unop_nd));

      lg(it);
    }


  auto  prexpr = read_primary_expression(it);

    if(prexpr)
    {
      nd->append_child(std::move(prexpr));

      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_binary_operator(token_iterator&  it)
{
  auto  opco = it->get_operator_code();

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
      return std::make_unique<ast_node>(*it++,"binary operator");
    } 


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_expression_element(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("expression element");

  auto   binop_nd = read_binary_operator(it);
  auto  unexpr_nd = read_unary_expression(it);

    if(binop_nd && unexpr_nd)
    {
      nd->append_child(std::move( binop_nd));
      nd->append_child(std::move(unexpr_nd));

      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_expression(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("expression");

  auto first_expr_nd = read_unary_expression(it);

    if(first_expr_nd)
    {
      nd->append_child(std::move(first_expr_nd));

        for(;;)
        {
          ast_loopguard  lg("read_expression",it);

          auto  expre_nd = read_expression_element(it);

            if(!expre_nd)
            {
              break;
            }


          nd->append_child(std::move(expre_nd));

          lg(it);
        }


      return std::move(nd);
    }


  return nullptr;
}




}




