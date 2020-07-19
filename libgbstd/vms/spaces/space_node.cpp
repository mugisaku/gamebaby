#include"libgbstd/vm.hpp"




namespace gbstd{




space_node::
space_node() noexcept:
m_root(this),
m_kind(kinds::global_space)
{
  new(&m_data) global_space(*this);
}




function&
space_node::
create_function(std::string_view  name, typesystem::function_signature&&  sig) noexcept
{
  auto&  nd = m_children.emplace_back(std::make_unique<space_node>());

  nd->m_parent =   this;
  nd->m_root   = m_root;

  new(&nd->m_data) function(*nd,name,std::move(sig));

  nd->m_kind = kinds::function;

  return nd->m_data.fn;
}


block_space&
space_node::
create_block_space() noexcept
{
  auto&  nd = m_children.emplace_back(std::make_unique<space_node>());

  nd->m_parent =   this;
  nd->m_root   = m_root;

  new(&nd->m_data) block_space(*nd);

  nd->m_kind = kinds::block_space;

  return nd->m_data.bsp;
}




space_node&
space_node::
clear() noexcept
{
    switch(m_kind)
    {
  case(kinds::global_space): std::destroy_at(&m_data.gsp);break;
  case(kinds::function    ): std::destroy_at(&m_data.fn);break;
  case(kinds::block_space ): std::destroy_at(&m_data.bsp);break;
    }


  m_kind = kinds::null;

  m_children.clear();

  return *this;
}


const function*
space_node::
find_function(std::string_view  name) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      const function*  fn = nullptr;

        if(nd->is_function())
        {
            if(nd->get_function().get_name() == name)
            {
               fn = &nd->get_function();
            }
        }

      else
        {
            for(auto&  child: m_children)
            {
                if(child->is_function() && (child->get_function().get_name() == name))
                {
                  fn = &child->get_function();

                  break;
                }
            }
        }


        if(fn)
        {
          return fn;
        }


      nd = nd->m_parent;
    }


  return nullptr;
}


type_info
space_node::
find_type_info(std::string_view  name) const noexcept
{
    if(name == std::string_view("int"))
    {
      return g_int_ti;
    }

  else
    if(name == std::string_view("float"))
    {
      return g_float_ti;
    }

  else
    if(name == std::string_view("bool"))
    {
      return g_bool_ti;
    }

  else
    if(name == std::string_view("intptr"))
    {
      return g_intptr_ti;
    }


  type_info  ti(name);

    if(ti)
    {
      return std::move(ti);
    }
  

  auto  nd = this;

    while(nd)
    {
        if(nd->is_function())
        {
          nd = nd->m_root;
        }

      else
        {
            if(nd->is_block_space())
            {
              ti = nd->m_data.bsp.find_type_info(name);
            }

          else
            if(nd->is_global_space())
            {
              ti = nd->m_data.gsp.find_type_info(name);
            }


            if(ti)
            {
              return ti;
            }


          nd = nd->m_parent;
        }
    }


  return type_info();
}


const variable_info*
space_node::
find_variable_info(std::string_view  name) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      const variable_info*  vi = nullptr;

        if(nd->is_function())
        {
          vi = nd->m_data.fn.find_parameter_variable_info(name);

            if(vi)
            {
              return vi;
            }


          nd = nd->m_root;
        }

      else
        {
            if(nd->is_block_space())
            {
              vi = nd->m_data.bsp.find_variable_info(name);
            }

          else
            if(nd->is_global_space())
            {
              vi = nd->m_data.gsp.find_variable_info(name);
            }


            if(vi)
            {
              return vi;
            }


          nd = nd->m_parent;
        }
    }


  return nullptr;
}


found_info
space_node::
find_all(std::string_view  name) const noexcept
{
  auto  ti = find_type_info(name);

    if(ti)
    {
      return {ti};
    }


  auto  fn = find_function(name);

    if(fn)
    {
      return {*fn};
    }


  auto  vi = find_variable_info(name);

    if(vi)
    {
      return {*vi};
    }


  return {};
}




void
space_node::
collect_functions(std::vector<std::reference_wrapper<function>>&  buf) const noexcept
{
    for(auto&  child: m_children)
    {
    }


    for(auto&  child: m_children)
    {
        if(child->is_function())
        {
          buf.emplace_back(child->get_function());
        }


      child->collect_functions(buf);
    }
}


void
space_node::
collect_global_variables(std::vector<std::reference_wrapper<variable_info>>&  buf) const noexcept
{
    for(auto&  child: m_children)
    {
        if(child->is_block_space())
        {
          auto&  var_ls = child->get_block_space().get_variable_info_table();

            for(auto&  var: var_ls)
            {
                if(var->is_global_scope())
                {
                  buf.emplace_back(*var);
                }
            }
        }


      child->collect_global_variables(buf);
    }
}




void
space_node::
print() const noexcept
{
    switch(m_kind)
    {
  case(kinds::global_space): m_data.gsp.print();break;
  case(kinds::function    ): m_data.fn.print();break;
  case(kinds::block_space ): m_data.bsp.print();break;
    }


  printf("{\n");

    for(auto&  child: m_children)
    {
      child->print();

      printf("\n");
    }


  printf("}\n");
}




}




