#include"libgbstd/sns.hpp"
#include<algorithm>



namespace gbstd{
namespace sns{




namespace{
auto  cmp =
[](const article&  a, const article&  b)->bool
{
  return a.get_timestamp() > b.get_timestamp();
};
}


void
timeline::
ready(timestamp  ts) noexcept
{
  m_head_ts = ts;
  m_tail_ts = ts;

    for(auto&  nd: m_nodes)
    {
      nd.reset();
    }
}


void
timeline::
read_forward() noexcept
{
  m_temporary_table.clear();

    for(auto&  nd: m_nodes)
    {
      auto  recptr = nd.get_next_head();

        if(recptr)
        {
          auto  ts = recptr->get_timestamp();

            if(ts > m_head_ts)
            {
              m_head_ts = ts;
            }
        }
    }


  m_temporary_table.clear();

    for(auto&  nd: m_nodes)
    {
        for(;;)
        {
          auto  recptr = nd.get_next_head();

            if(recptr)
            {
              auto  ts = recptr->get_timestamp();

                if(ts < m_head_ts)
                {
                  break;
                }


              m_temporary_table.emplace_back(nd.get_observer(),*recptr);

              nd.advance_head_index();
            }

          else
            {
              break;
            }
        }
    }


    if(m_temporary_table.size())
    {
      auto  p = m_temporary_table.data();
      auto  n = m_temporary_table.size();

      std::stable_sort(p,p+n,cmp);

        for(auto&  art: m_main_table)
        {
          m_temporary_table.emplace_back(std::move(art));
        }


      std::swap(m_main_table,m_temporary_table);
    }
}


void
timeline::
read_backward() noexcept
{
    for(auto&  nd: m_nodes)
    {
      auto  recptr = nd.get_next_tail();

        if(recptr)
        {
          auto  ts = recptr->get_timestamp();

            if(ts < m_tail_ts)
            {
              m_tail_ts = ts;
            }
        }
    }


  m_temporary_table.clear();

    for(auto&  nd: m_nodes)
    {
        for(;;)
        {
          auto  recptr = nd.get_next_tail();

            if(recptr)
            {
              auto  ts = recptr->get_timestamp();

                if(ts < m_tail_ts)
                {
                  break;
                }


              m_temporary_table.emplace_back(nd.get_observer(),*recptr);

              nd.advance_pretail_index();
            }

          else
            {
              break;
            }
        }
    }


    if(m_temporary_table.size())
    {
      auto  p = m_temporary_table.data();
      auto  n = m_temporary_table.size();

      std::stable_sort(p,p+n,cmp);

        for(auto&  art: m_temporary_table)
        {
          m_main_table.emplace_back(std::move(art));
        }
    }
}




}}




