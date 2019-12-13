#include"libgbstd/vm.hpp"




namespace gbstd{




value
phi_entry::
evaluate(evaluation_context&  ctx) const noexcept
{
  return m_operand.evaluate(ctx);
}


}




