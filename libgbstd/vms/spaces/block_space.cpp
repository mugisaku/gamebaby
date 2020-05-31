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
allocate_address(address_t&  global_end, address_t&  local_end, address_t&  operation_stack_size) noexcept
{
    for(auto&  mi: m_memo_info_table)
    {
      local_end = mi->set_address(local_end);
    }


    for(auto&  child: m_node.get_children())
    {
        if(child->is_function())
        {
          child->get_function().allocate_address(global_end);
        }

      else
        if(child->is_block_space())
        {
          child->get_block_space().allocate_address(global_end,local_end,operation_stack_size);
        }
    }


    for(auto&  st: m_statement_list)
    {
/*
      pointer_wrapper<expression>  ep = st.is_return()?     &st.get_return().get_expression()
                                       :st.is_expression()? &st.get_expression()
                                       :nullptr
                                       ;


        if(ep)
        {
          auto  sz = ep->allocate_address(get_node(),0);

          operation_stack_size = std::max(operation_stack_size,sz);
        }
*/
    }
}


void
block_space::
print() const noexcept
{
  basic_space::print();
}




}




