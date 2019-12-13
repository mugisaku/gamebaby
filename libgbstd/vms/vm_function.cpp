#include"libgbstd/vm.hpp"




namespace gbstd{




block&
function::
create_block(std::string_view  label) noexcept
{
  auto  ptr = new block(*this,label);

  m_blocks.emplace_back(ptr);

  return *ptr;
}


value
function::
evaluate(std::string_view  label) const noexcept
{
/*
    for(auto&  blk: m_blocks)
    {
      auto  cl = blk->find_codeline(label);

        if(cl)
        {
            if(cl->is_store_instruction())
            {
              cl->get_store_instruction().execute(*this);
            }

          else
            if(cl->is_branch_instruction())
            {
              cl->get_branch_instruction().execute(*this);
            }

          else
            if(cl->is_return_instruction())
            {
              cl->get_return_instruction().execute(*this);
            }

          else
            if(cl->is_unary_operation())
            {
              return cl->get_unary_operation().evaluate(*this);
            }

          else
            if(cl->is_binary_operation())
            {
              return cl->get_binary_operation().evaluate(*this);
            }

          else
            if(cl->is_phi_operation())
            {
              return cl->get_phi_operation().evaluate(*this);
            }

          else
            if(cl->is_call_operation())
            {
              return cl->get_call_operation().evaluate(*this);
            }


          return value::make_null();
        }
    }
*/

  return value::make_null();
}




}




