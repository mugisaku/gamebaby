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

        if(nd->is_block_space())
        {
          fn = nd->m_data.bsp.find_function(name);
        }

      else
        if(nd->is_global_space())
        {
          fn = nd->m_data.gsp.find_function(name);
        }

      else
        if(nd->is_function())
        {
        }


        if(fn)
        {
          return fn;
        }


      nd = nd->m_parent;
    }


  return nullptr;
}


const type_info*
space_node::
find_type_info_by_name(std::string_view  name) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      const type_info*  ti = nullptr;

        if(nd->is_function())
        {
          nd = nd->m_root;
        }

      else
        {
            if(nd->is_block_space())
            {
              ti = nd->m_data.bsp.find_type_info_by_name(name);
            }

          else
            if(nd->is_global_space())
            {
              ti = nd->m_data.gsp.find_type_info_by_name(name);
            }


            if(ti)
            {
              return ti;
            }


          nd = nd->m_parent;
        }
    }


  return nullptr;
}


const type_info*
space_node::
find_type_info_by_id(std::string_view  id) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      const type_info*  ti = nullptr;

        if(nd->is_function())
        {
          nd = nd->m_root;
        }

      else
        {
            if(nd->is_block_space())
            {
              ti = nd->m_data.bsp.find_type_info_by_id(id);
            }

          else
            if(nd->is_global_space())
            {
              ti = nd->m_data.gsp.find_type_info_by_id(id);
            }


            if(ti)
            {
              return ti;
            }


          nd = nd->m_parent;
        }
    }


  return nullptr;
}


const memo_info*
space_node::
find_memo_info(std::string_view  name) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      const memo_info*  mi = nullptr;

        if(nd->is_function())
        {
          mi = nd->m_data.fn.find_parameter_memo_info(name);

            if(mi)
            {
              return mi;
            }


          nd = nd->m_root;
        }

      else
        {
            if(nd->is_block_space())
            {
              mi = nd->m_data.bsp.find_memo_info(name);
            }

          else
            if(nd->is_global_space())
            {
              mi = nd->m_data.gsp.find_memo_info(name);
            }


            if(mi)
            {
              return mi;
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
  auto  ti = find_type_info_by_name(name);

    if(ti)
    {
      return {*ti};
    }


  auto  fn = find_function(name);

    if(fn)
    {
      return {*fn};
    }


  auto  mi = find_memo_info(name);

    if(mi)
    {
      return {*mi};
    }


  return {};
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




