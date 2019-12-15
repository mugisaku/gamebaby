#include"libgbstd/vm.hpp"




namespace gbstd{




codeline&
block::
create_codeline() noexcept
{
  m_codelines.emplace_back(*this);

  return m_codelines.back();
}


const operation*
block::
find_operation(std::string_view  label) const noexcept
{
    for(auto&  cl: m_codelines)
    {
        if(cl.is_operation())
        {
          auto&  op = cl.get_operation();

            if(op.get_label() == label)
            {
              return &op;
            }
        }
    }


  return nullptr;
}




}




