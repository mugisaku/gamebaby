#include"libgbstd/vm.hpp"




namespace gbstd{




value
call_operation::
evaluate(execution_frame&  frm) const noexcept
{
  auto  target = frm.get_function().get_space().find_function(m_function_name);

    if(target)
    {
      execution_frame  cofrm(*target,&frm);

      return cofrm.run();
    }


  return value::make_null();
}




}




