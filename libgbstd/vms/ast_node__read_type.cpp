#include"libgbstd/vms/ast.hpp"




namespace gbstd{




std::unique_ptr<ast_node>
ast_node::
read_struct_def(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("struct definition");

  ++it;

    for(;;)
    {
      ast_loopguard  lg("read_struct_def",it);

        if(it->is_operator_code("}"))
        {
          ++it;

          return std::move(nd);
        }


      auto  para_nd = read_parameter(it);

        if(para_nd)
        {
          nd->append_child(std::move(para_nd));

            if(it->is_operator_code(";"))
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
read_struct(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("struct");

  auto  name_nd = read_name(++it);

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

        if(it->is_operator_code("{"))
        {
          nd->append_child(read_struct_def(it));
        }


      return std::move(nd);
    }


  throw ast_error("struct is declared without name\n",*it);
}


std::unique_ptr<ast_node>
ast_node::
read_union_def(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("union definition");

  ++it;

    for(;;)
    {
      ast_loopguard  lg("read_union_def",it);

        if(it->is_operator_code("}"))
        {
          ++it;

          return std::move(nd);
        }


      auto  para_nd = read_parameter(it);

        if(para_nd)
        {
          nd->append_child(std::move(para_nd));

            if(it->is_operator_code(";"))
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
read_union(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("union");

  auto  name_nd = read_name(++it);

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

        if(it->is_operator_code("{"))
        {
          nd->append_child(read_union_def(it));
        }


      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_enumerator(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("enumerator");

  auto  name_nd = read_name(++it);

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

        if(it->is_operator_code("="))
        {
          auto  expr_nd = read_expression(++it);

            if(expr_nd)
            {
              nd->append_child(std::move(expr_nd));

              return std::move(nd);
            }
        }

      else
        {
          return std::move(nd);
        }
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_enum_def(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("enum definition");

  ++it;

    for(;;)
    {
      ast_loopguard  lg("read_enum_def",it);

      auto  en_nd = read_enumerator(it);

        if(en_nd)
        {
          nd->append_child(std::move(en_nd));
        }

      else
        if(it->is_operator_code(","))
        {
          ++it;
        }

      else
        if(it->is_operator_code("}"))
        {
          ++it;

          return std::move(nd);
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
read_enum(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("enum");

  auto  name_nd = read_name(++it);

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

        if(it->is_operator_code("{"))
        {
          nd->append_child(read_enum_def(it));
        }


      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_alias(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("alias");

  auto  type_nd = read_type(++it);
  auto  name_nd = read_name(  it);

    if(type_nd && name_nd)
    {
      nd->append_child(std::move(type_nd));
      nd->append_child(std::move(name_nd));

      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_type(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("type");

  auto  base_nd = read_name(it);

    if(base_nd)
    {
      nd->append_child(std::move(base_nd));

        for(;;)
        {
          ast_loopguard  lg("read_type",it);

            if(it->is_operator_code("*"))
            {
              nd->append_child(std::make_unique<ast_node>(*it++,"pointer"));
            }

          else
            if(it->is_operator_code("&"))
            {
              nd->append_child(std::make_unique<ast_node>(*it++,"reference"));

              break;
            }

          else
            if(it->is_operator_code("["))
            {
              nd->append_child(read_index(it));
            }

          else
            {
              break;
            }


          lg(it);
        }


      return std::move(nd);
    }


  return nullptr;
}




}




