#include"libgbstd/vm.hpp"




namespace gbstd{




codeline&
block::
create_codeline(std::string_view  lb) noexcept
{
  m_codelines.emplace_back(*this,lb);

  return m_codelines.back();
}


const codeline*
block::
find_codeline(std::string_view  label) const noexcept
{
    for(auto&  cl: m_codelines)
    {
        if(cl.get_label() == label)
        {
          return &cl;
        }
    }


  return nullptr;
}




}




