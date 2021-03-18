#include"libgbstd/vms/ast.hpp"
#include"libgbstd/typesystem.hpp"




namespace gbstd{


using namespace typesystem;


std::unique_ptr<ast_node>
ast_builder::
read_struct_def()
{
  auto  nd = make_ast_node("struct definition");

  advance();

    for(;;)
    {
      ast_loopguard  lg("read_struct_def",m_iterator);

        if(m_iterator->is_operator_code("}"))
        {
          advance();

          return std::move(nd);
        }


      auto  para_nd = read_parameter();

        if(para_nd)
        {
          nd->append_child(std::move(para_nd));

            if(m_iterator->is_operator_code(";"))
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
read_struct()
{
  auto  nd = make_ast_node("struct");

  advance();

  auto  name_nd = read_name();

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

      nd->content().set_data(new typesystem::type_info());

        if(m_iterator->is_operator_code("{"))
        {
          nd->append_child(read_struct_def());
        }


      return std::move(nd);
    }


  throw ast_error("struct is declared without name\n",*m_iterator);
}


std::unique_ptr<ast_node>
ast_builder::
read_union_def()
{
  auto  nd = make_ast_node("union definition");

  advance();

    for(;;)
    {
      ast_loopguard  lg("read_union_def",m_iterator);

        if(m_iterator->is_operator_code("}"))
        {
          advance();

          return std::move(nd);
        }


      auto  para_nd = read_parameter();

        if(para_nd)
        {
          nd->append_child(std::move(para_nd));

            if(m_iterator->is_operator_code(";"))
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
read_union()
{
  auto  nd = make_ast_node("union");

  advance();

  auto  name_nd = read_name();

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

      nd->content().set_data(new typesystem::type_info());

        if(m_iterator->is_operator_code("{"))
        {
          nd->append_child(read_union_def());
        }


      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_enumerator()
{
  auto  nd = make_ast_node("enumerator");

  advance();

  auto  name_nd = read_name();

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

        if(m_iterator->is_operator_code("="))
        {
          advance();

          auto  expr_nd = read_expression();

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
ast_builder::
read_enum_def()
{
  auto  nd = make_ast_node("enum definition");

  advance();

    for(;;)
    {
      ast_loopguard  lg("read_enum_def",m_iterator);

      auto  en_nd = read_enumerator();

        if(en_nd)
        {
          nd->append_child(std::move(en_nd));
        }

      else
        if(m_iterator->is_operator_code(","))
        {
          advance();
        }

      else
        if(m_iterator->is_operator_code("}"))
        {
          advance();

          return std::move(nd);
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
read_enum()
{
  auto  nd = make_ast_node("enum");

  advance();

  auto  name_nd = read_name();

    if(name_nd)
    {
      nd->append_child(std::move(name_nd));

      nd->content().set_data(new typesystem::type_info());

        if(m_iterator->is_operator_code("{"))
        {
          nd->append_child(read_enum_def());
        }


      return std::move(nd);
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_builder::
read_alias()
{
  auto  nd = make_ast_node("alias");

  advance();

  auto  type_nd = read_type();
  auto  name_nd = read_name();

    if(type_nd && name_nd)
    {
      nd->append_child(std::move(type_nd));
      nd->append_child(std::move(name_nd));

      return std::move(nd);
    }


  return nullptr;
}


namespace{
constexpr int  g_word_size = 8;
}


std::unique_ptr<ast_node>
ast_builder::
read_type()
{
  auto  nd = make_ast_node("type");

  auto  name_nd = read_name();

    if(name_nd)
    {
      auto&  s = name_nd->content().get_token()->get_string();

      type_info  ti = find_type_info(s);

        if(!ti)
        {
          throw ast_error(form_string("%s as typename is not found.",s.data()));
        }


      nd->append_child(std::move(name_nd));

        for(;;)
        {
          ast_loopguard  lg("read_type",m_iterator);

            if(m_iterator->is_operator_code("*"))
            {
              ti = ti.form_typed_pointer(g_word_size);

              nd->append_child(make_ast_node(*m_iterator++,"pointer"));
            }

          else
            if(m_iterator->is_operator_code("&"))
            {
              ti = ti.form_reference(g_word_size);

              nd->append_child(make_ast_node(*m_iterator++,"reference"));

              break;
            }

          else
            if(m_iterator->is_operator_code("["))
            {
              auto  idx_nd = read_index();

              auto  expr_nd = idx_nd->find_child("expression");

              auto  cons = expr_nd->content().get_data<ast_constant>();

                if(cons)
                {
                    if((*cons)->is_integer())
                    {
                      ti = ti.form_array(cons->get_i64());

                      nd->append_child(std::move(idx_nd));
                    }

                  else
                    {
                      report;
                    }
                }

              else
                {
                  throw ast_error("index expression is not constant\n",*m_iterator);
                }
            }

          else
            {
              break;
            }


          lg(m_iterator);
        }


      nd->content().set_data(new type_info(std::move(ti)));

      return std::move(nd);
    }


  return nullptr;
}




}




