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


bool
timeline_node::
initialize() noexcept
{
    if(!m_status.test(flags::initialize))
    {
      auto  n = m_observer->get_record_table().get_number_of_records();

        if(n)
        {
                            m_head_index = n-1;
          m_pretail_index = m_head_index      ;

          m_status.set(flags::initialize);
        }
    }


  return m_status.test(flags::initialize);
}


timeline_node&
timeline_node::
reset() noexcept
{
  m_status.clear();

  return *this;
}




void
timeline_node::
advance_pretail_index() noexcept
{
    if(!m_status.test(flags::reach_bottom))
    {
        if(!m_pretail_index)
        {
          m_status.set(flags::reach_bottom);
        }

      else
        {
          --m_pretail_index;
        }
    }
}


const record*
timeline_node::
get_next_head() noexcept
{
  auto&  tbl = m_observer->get_record_table();

  return (initialize() && (m_head_index < (tbl.get_number_of_records()-1)))?  &tbl[m_head_index+1]
        :nullptr;
}


const record*
timeline_node::
get_next_tail() noexcept
{
  auto&  tbl = m_observer->get_record_table();

  const record*  rec = nullptr;

    if(initialize() && !m_status.test(flags::reach_bottom))
    {
      rec = &tbl[m_pretail_index];
    }


  return rec;
}




}}




