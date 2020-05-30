#include"libgbstd/vm.hpp"




namespace gbstd{




space_node::
space_node() noexcept:
m_root(this)
{
}


space_node::
space_node(space_node&  parent) noexcept:
m_parent(&parent),
m_root(parent.m_root)
{
}




space_node&
space_node::
assign(global_space&  gsp) noexcept
{
  m_data.gsp = &gsp;

  m_kind = kinds::global_space;

  return *this;
}


space_node&
space_node::
assign(function&  fn) noexcept
{
  m_data.fn = &fn;

  m_kind = kinds::function;

  return *this;
}


space_node&
space_node::
assign(block_space&   bsp) noexcept
{
  m_data.bsp = &bsp;

  m_kind = kinds::block_space;

  return *this;
}




space_node&
space_node::
create_child() noexcept
{
  return *m_children.emplace_back(std::make_unique<space_node>(*this));
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
          fn = nd->m_data.bsp->find_function(name);
        }

      else
        if(nd->is_global_space())
        {
          fn = nd->m_data.gsp->find_function(name);
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
              ti = nd->m_data.bsp->find_type_info_by_name(name);
            }

          else
            if(nd->is_global_space())
            {
              ti = nd->m_data.gsp->find_type_info_by_name(name);
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
              ti = nd->m_data.bsp->find_type_info_by_id(id);
            }

          else
            if(nd->is_global_space())
            {
              ti = nd->m_data.gsp->find_type_info_by_id(id);
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
          mi = nd->m_data.fn->find_parameter_memo_info(name);

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
              mi = nd->m_data.bsp->find_memo_info(name);
            }

          else
            if(nd->is_global_space())
            {
              mi = nd->m_data.gsp->find_memo_info(name);
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
  case(kinds::global_space): m_data.gsp->print();break;
  case(kinds::function    ): m_data.fn->print();break;
  case(kinds::block_space ): m_data.bsp->print();break;
    }
}




}




