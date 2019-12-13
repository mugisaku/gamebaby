#include"libgbstd/vm.hpp"




namespace gbstd{




value
phi_operation::
evaluate(evaluation_context&  ctx) const noexcept
{
    for(auto&  ent: m_entries)
    {
//        if(ent.get_label() == blk.get_label())
        {
          return ent.evaluate(ctx);
        }
    }


  return value::make_null();
}




}




