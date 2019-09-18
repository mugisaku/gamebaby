#include"libgbstd/sns.hpp"




namespace gbstd{
namespace sns{




index_list&
index_list::
assign(range   r) noexcept
{
  m_container.clear();

    if(r)
    {
        for(auto  i = r.bottom;   i <= r.top;  ++i)
        {
          m_container.emplace_back(i);
        }
    }


  return *this;
}




}}




