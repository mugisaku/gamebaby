#include"libgbstd/vm.hpp"




namespace gbstd{




value
call_operation::
operator()(execution_frame&  frm) const noexcept
{
  auto  target = frm.get_function().get_space().find_function(m_function_name);

    if(target)
    {
      auto  argtypes = target->get_signature().get_parameter_list().begin();
      auto  argnames = target->get_argument_name_list().begin();

      std::vector<variable>  args;

        for(auto&  o: m_arguments)
        {
          args.emplace_back(**argtypes++,*argnames++);

          args.back().get_value() = o.evaluate(frm);
        }


      auto  cofrm = std::make_unique<execution_frame>(*target,&frm,args.size(),args.data());

      return cofrm->run();
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




