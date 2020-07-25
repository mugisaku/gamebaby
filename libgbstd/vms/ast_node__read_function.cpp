#include"libgbstd/vms/ast.hpp"




namespace gbstd{




std::unique_ptr<ast_node>
ast_node::
read_local_space(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("local space");

  ++it;

    while(it)
    {
      ast_loopguard  lg("read_local_space",it);

        if(it->is_operator_code("}"))
        {
          ++it;

          return std::move(nd);
        }

      else
        if(it->is_operator_code(";"))
        {
          ++it;
        }

      else
        {
          auto  stmt_nd = read_statement(it);

            if(stmt_nd)
            {
              nd->append_child(std::move(stmt_nd));
            }

          else
            {
              throw ast_error("statement read error",*it);
            }
        }


      lg(it);
    }


  throw ast_error("local space read error",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_parameter(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("parameter");

  auto  type_nd = read_type(it);
  auto  name_nd = read_name(it);

    if(type_nd)
    {
      nd->append_child(std::move(type_nd));

        if(name_nd)
        {
          nd->append_child(std::move(name_nd));
        }


      return std::move(nd);
    }


  throw ast_error("parameter read error",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_parameter_list(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("parameter list");

  ++it;

    for(;;)
    {
      ast_loopguard  lg("read_parameter_list",it);

        if(it->is_operator_code(")"))
        {
          ++it;

          return std::move(nd);
        }


      auto  para_nd = read_parameter(it);

        if(para_nd)
        {
          nd->append_child(std::move(para_nd));

            if(it->is_operator_code(","))
            {
              ++it;
            }
        }


      lg(it);
    }


  throw ast_error("parameter list read error",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_function(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("function");

  auto   fn_name_nd = read_name(++it);
  auto  ret_type_nd = read_type(  it);

    if(fn_name_nd && ret_type_nd && it->is_operator_code("("))
    {
      auto  paras_nd = read_parameter_list(it);

        if(paras_nd)
        {
          nd->append_child(std::move( fn_name_nd));
          nd->append_child(std::move(ret_type_nd));
          nd->append_child(std::move(   paras_nd));

            if(it->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space(it);

                if(!sp_nd)
                {
                  throw ast_error("read error at function body",*it);
                }


              nd->append_child(std::move(sp_nd));
            }


          return std::move(nd);
        }
    }
    

  throw ast_error("read error at function head",*it);
}




}




