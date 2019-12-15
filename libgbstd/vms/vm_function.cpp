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


const block*
function::
find_block(std::string_view  label) const noexcept
{
    for(auto&  blk: m_blocks)
    {
        if(blk->get_label() == label)
        {
          return blk.get();
        }
    }


  return nullptr;
}


const operation*
function::
find_operation(std::string_view  label) const noexcept
{
    for(auto&  blk: m_blocks)
    {
      auto  op = blk->find_operation(label);

        if(op)
        {
          return op;
        }
    }


  return nullptr;
}




}




