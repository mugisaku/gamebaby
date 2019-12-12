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




}




