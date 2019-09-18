#include"libgbstd/sns.hpp"




namespace gbstd{
namespace sns{




const account_observer&
platform::
create_account(timestamp  ts) noexcept
{
  auto  n = m_account_table.size();

  auto  ptr = std::make_unique<account>(n,ts);

  auto&  obs = ptr->get_observer();

  m_account_table.emplace_back(std::move(ptr));

  return obs;
}


void
platform::
print() const noexcept
{
    for(auto&  ptr: m_account_table)
    {
      ptr->print();

      printf("\n");
    }
}




}}




