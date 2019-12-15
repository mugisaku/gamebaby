#include"libgbstd/vm.hpp"




namespace gbstd{




value
call_operation::
evaluate(evaluation_context&  ctx) const noexcept
{
  auto  target = ctx->get_function().get_space().find_function(m_function_name);

    if(target)
    {
//      return target->start(m_arguments);
    }


  return value::make_null();
}




}




