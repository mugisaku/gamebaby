#include"libgbstd/vms/ast.hpp"




namespace gbstd{




std::unique_ptr<ast_node>
ast_builder::
read_local_space()
{
  auto  root_nd = make_ast_node("local space");

  push(root_nd);

  advance();

    while(m_iterator)
    {
      ast_loopguard  lg("read_local_space",m_iterator);

        if(m_iterator->is_operator_code("}"))
        {
          advance();

          pop();

          return std::move(root_nd);
        }

      else
        {
          root_nd->append_child(read_statement());
        }


      lg(m_iterator);
    }


  throw ast_error("local space read error",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_parameter()
{
  auto  nd = make_ast_node("parameter");

  auto  type_nd = read_type();
  auto  name_nd = read_name();

    if(type_nd)
    {
      nd->append_child(std::move(type_nd));

        if(name_nd)
        {
          nd->append_child(std::move(name_nd));
        }


      return std::move(nd);
    }


  throw ast_error("parameter read error",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_parameter_list()
{
  auto  nd = make_ast_node("parameter list");

  advance();

    for(;;)
    {
      ast_loopguard  lg("read_parameter_list",m_iterator);

        if(m_iterator->is_operator_code(")"))
        {
          advance();

          return std::move(nd);
        }


      auto  para_nd = read_parameter();

        if(para_nd)
        {
          nd->append_child(std::move(para_nd));

            if(m_iterator->is_operator_code(","))
            {
              advance();
            }
        }


      lg(m_iterator);
    }


  throw ast_error("parameter list read error",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_function()
{
  auto  root_nd = make_ast_node("function");

  push(root_nd);

  advance();

  auto   fn_name_nd = read_name();
  auto  ret_type_nd = read_type();

    if(fn_name_nd && ret_type_nd && m_iterator->is_operator_code("("))
    {
      auto  paras_nd = read_parameter_list();

        if(paras_nd)
        {
          root_nd->append_child(std::move( fn_name_nd));
          root_nd->append_child(std::move(ret_type_nd));
          root_nd->append_child(std::move(   paras_nd));

            if(m_iterator->is_operator_code("{"))
            {
              auto  sp_nd = read_local_space();

                if(!sp_nd)
                {
                  throw ast_error("read error at function body",*m_iterator);
                }


              root_nd->append_child(std::move(sp_nd));
            }


          pop();

          return std::move(root_nd);
        }
    }
    

  throw ast_error("read error at function head",*m_iterator);
}




}




