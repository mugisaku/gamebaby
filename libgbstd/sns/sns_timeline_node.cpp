#include"libgbstd/sns.hpp"




namespace gbstd{
namespace sns{




timeline_node&
timeline_node::
set_observer(const account_observer&  obs) noexcept
{
  m_observer = obs;

  return *this;
}


timeline_node&
timeline_node::
reset() noexcept
{
  auto  n = m_observer->get_record_table().get_number_of_records();

  m_head_index    = n? n-1:0;
  m_pretail_index = m_head_index;


  return *this;
}


const record*
timeline_node::
get_next_head() noexcept
{
  auto&  tbl = m_observer->get_record_table();

  return (m_head_index < (tbl.get_number_of_records()-1))?  &tbl[m_head_index+1]
        :nullptr;
}


const record*
timeline_node::
get_next_tail() noexcept
{
  auto&  tbl = m_observer->get_record_table();

  return (~m_pretail_index)? &tbl[m_pretail_index]
        :nullptr;
}




}}




