#include"libgbstd/vm.hpp"




namespace gbstd{




value
call_operation::
evaluate(const space&  sp) const noexcept
{
  auto  fn = sp.find_function(m_function_name);

    if(fn)
    {
      return fn->start(m_arguments);
    }


  return value::make_null();
}




}




