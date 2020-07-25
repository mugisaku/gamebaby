#include"libgbstd/vms/ast.hpp"




namespace gbstd{




void
ast_node::
append_child(std::unique_ptr<ast_node>&&  e) noexcept
{
    if(!e)
    {
      printf("%s< null node is not be child\n",m_kind_id.data());

      return;
    }


    if(e->get_parent())
    {
      printf("%s< %s has parent\n",e->get_kind_id().data(),m_kind_id.data());

      return;
    }


  e->m_parent = this;

  m_children.emplace_back(std::move(e));
}




std::unique_ptr<ast_node>
ast_node::
read_name(token_iterator&  it)
{
    if(it->is_identifier())
    {
      return std::make_unique<ast_node>(*it++,"name");
    }


  return nullptr;
}


std::unique_ptr<ast_node>
ast_node::
read_global_space(token_iterator&  it)
{
  auto  nd = std::make_unique<ast_node>("");

    while(it)
    {
      ast_loopguard  lg("read_global_space",it);

        if(it->is_operator_code(";"))
        {
          ++it;
        }

      else
        if(it->is_identifier())
        {
          auto&  s = it->get_string();

            if(s == std::string_view("function"))
            {
              nd->append_child(read_function(it));
            }

          else
            if(s == std::string_view("struct"))
            {
              nd->append_child(read_struct(it));
            }

          else
            if(s == std::string_view("enum"))
            {
              nd->append_child(read_enum(it));
            }

          else
            if(s == std::string_view("union"))
            {
              nd->append_child(read_union(it));
            }

          else
            if(s == std::string_view("alias"))
            {
              nd->append_child(read_alias(it));
            }

          else
            if(s == std::string_view("let"))
            {
              nd->append_child(read_let(it));
            }
        }

      else
        {
          throw ast_error("read_global_space error",*it);
        }


      lg(it);
    }


  return std::move(nd);
}


void
ast_node::
print_internal(std::vector<std::string>&  route) const noexcept
{
    if(m_token)
    {
      m_token->print();

      printf(":");
    }


  route.emplace_back(m_kind_id);

    if(m_token || m_children.empty())
    {
        for(auto&  s: route)
        {
          printf("/%s",s.data());
        }


      printf("\n");
    }


    for(auto&  child: m_children)
    {
      child->print_internal(route);
    }


  route.pop_back();
}


void
ast_node::
print() const noexcept
{
  std::vector<std::string>  route;

  print_internal(route);
}




}




