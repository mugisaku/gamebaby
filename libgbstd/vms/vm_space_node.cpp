#include"libgbstd/vm.hpp"




namespace gbstd{




space_node::
space_node() noexcept:
m_kind(kinds::global_space),
m_root(this)
{
  new(&m_data) global_space(*this);
}


space_node::
space_node(std::string_view  src):
m_kind(kinds::global_space),
m_root(this)
{
  new(&m_data) global_space(*this,src);
}


space_node::
space_node(space_node&  parent, std::string_view  name, function_signature&&  fnsig) noexcept:
m_kind(kinds::function),
m_parent(&parent),
m_root(parent.m_root)
{
  new(&m_data) function(*this,name,std::move(fnsig));
}


space_node::
space_node(space_node&  parent, nullptr_t) noexcept:
m_kind(kinds::block),
m_parent(&parent),
m_root(parent.m_root)
{
  new(&m_data) block_statement(*this);
}


space_node::
~space_node()
{
    switch(m_kind)
    {
  case(kinds::global_space): std::destroy_at(&m_data.gsp);break;
  case(kinds::function    ): std::destroy_at(&m_data.fn);break;
  case(kinds::block       ): std::destroy_at(&m_data.blk);break;
    }
}




pointer_wrapper<function>
space_node::
create_function(std::string_view  name, function_signature&&  fnsig) noexcept
{
  auto&  nd = m_children.emplace_back(std::make_unique<space_node>(*this,name,std::move(fnsig)));

  auto&  fn = nd->get_function();

    if(is_global_space())
    {
      m_data.gsp.push_function(fn);

      return &fn;
    }

  else
    if(is_function())
    {
    }

  else
    if(is_block())
    {
      m_data.blk.push_function(fn);

      return &fn;
    }


  return nullptr;
}


pointer_wrapper<block_statement>
space_node::
create_block() noexcept
{
  auto&  nd = m_children.emplace_back(std::make_unique<space_node>(*this,nullptr));

  auto&  blk = nd->get_block();

    if(is_global_space())
    {
    }

  else
    if(is_function())
    {
    }

  else
    if(is_block())
    {
      m_data.blk.push_statement(statement(blk));

      return &blk;
    }


  return nullptr;
}




bool
space_node::
push_type_info(type_info&&  ti) noexcept
{
    if(is_global_space())
    {
      m_data.gsp.push_type_info(std::move(ti));

      return true;
    }

  else
    if(is_block())
    {
      m_data.blk.push_type_info(std::move(ti));

      return true;
    }


  return false;
}


bool
space_node::
push_memo_info(const type_info&  ti, std::string_view  name) noexcept
{
    if(is_global_space())
    {
      m_data.gsp.push_memo_info(std::move(ti),name);

      return true;
    }

  else
    if(is_function())
    {
      m_data.fn.push_memo_info(std::move(ti),name);

      return true;
    }

  else
    if(is_block())
    {
      m_data.blk.push_memo_info(std::move(ti),name);

      return true;
    }


  return false;
}


bool
space_node::
push_statement(statement&&  st) noexcept
{
    if(is_block())
    {
      m_data.blk.push_statement(std::move(st));

      return true;
    }


  return false;
}


const function*
space_node::
find_function(std::string_view  name) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      const function*  fn = nullptr;

        if(nd->is_block())
        {
          fn = m_data.blk.find_function(name);
        }

      else
        if(nd->is_global_space())
        {
          fn = m_data.gsp.find_function(name);
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
find_type_info_by_name(std::string_view  name) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      type_info  ti;

        if(nd->is_block())
        {
          ti = m_data.blk.find_type_info_by_name(name);
        }

      else
        if(nd->is_global_space())
        {
          ti = m_data.gsp.find_type_info_by_name(name);
        }


        if(ti)
        {
          return ti;
        }


      nd = nd->m_parent;
    }


  return type_info();
}


type_info
space_node::
find_type_info_by_id(std::string_view  id) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      type_info  ti;

        if(nd->is_block())
        {
          ti = m_data.blk.find_type_info_by_id(id);
        }

      else
        if(nd->is_global_space())
        {
          ti = m_data.gsp.find_type_info_by_id(id);
        }


        if(ti)
        {
          return ti;
        }


      nd = nd->m_parent;
    }


  return type_info();
}


const memo_info*
space_node::
find_memo_info(std::string_view  name) const noexcept
{
  auto  nd = this;

    while(nd)
    {
      const memo_info*  mi = nullptr;

        if(nd->is_block())
        {
          mi = m_data.blk.find_memo_info(name);
        }

      else
        if(nd->is_global_space())
        {
          mi = m_data.gsp.find_memo_info(name);
        }


        if(mi)
        {
          return mi;
        }


      nd = nd->m_parent;
    }


  return nullptr;
}




void
space_node::
print() const noexcept
{
    switch(m_kind)
    {
  case(kinds::global_space): m_data.gsp.print();break;
  case(kinds::function    ): m_data.fn.print();break;
  case(kinds::block       ): m_data.blk.print();break;
    }
}




}




