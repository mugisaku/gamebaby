#include"libgbstd/vm.hpp"




namespace gbstd{




value
call_operation::
operator()(execution_frame&  frm) const noexcept
{
  auto  target = frm.get_function().get_space().find_function(m_function_name);

    if(target)
    {
//      auto  cofrm = std::make_unique<execution_frame>(*target,&frm,m_arguments.size(),m_arguments.data());

//      return cofrm->run();
    }


  return value();
}


void
call_operation::
print() const noexcept
{
  printf("cal %s ",m_function_name.data());

    for(auto&  v: m_arguments)
    {
      v.print();

      printf(" ");
    }
}




}




