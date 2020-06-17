#include"libgbstd/vm.hpp"




namespace gbstd{




const statement&
block_space::
operator[](int  i) const noexcept
{
  static const statement  null;

  return (i < m_statement_list.size())? m_statement_list[i]:null;
}


void
block_space::
clear() noexcept
{
  basic_space::clear();

  m_statement_list.clear();
}


block_space&
block_space::
create_block_space() noexcept
{
  return *m_children.emplace_back(std::make_unique<block_space>(m_node));
}


const function*
block_space::
get_parent_function() const noexcept
{
  auto  p = get_node().get_parent();

    while(p)
    {
        if(p->is_function())
        {
          return &p->get_function();
        }


      p = p->get_parent();
    }


  return nullptr;
}


void
block_space::
compile(compile_context&  ctx) const
{
    for(auto&  st: m_statement_list)
    {
      st.compile(m_node,ctx);
    }
}


void
block_space::
print() const noexcept
{
  basic_space::print();
}




}




