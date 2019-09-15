#include"libsns/sns.hpp"
#include<algorithm>




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
fetch_forward(timestamp  ts) noexcept
{
  m_temporary_table.clear();

    for(auto&  nd: m_nodes)
    {
        if(nd)
        {
            for(;;)
            {
              auto  ptr = nd.fetch_head(ts);

                if(!ptr)
                {
                  break;
                }


              m_temporary_table.emplace_back(nd.get_observer(),*ptr);
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
fetch_backward() noexcept
{
    for(auto&  nd: m_nodes)
    {
        if(nd)
        {
          auto  ts = nd.get_next_tail_timestamp();

            if(ts < m_tail_ts)
            {
              m_tail_ts = ts;
            }
        }
    }


  m_temporary_table.clear();

    for(auto&  nd: m_nodes)
    {
        if(nd)
        {
            for(;;)
            {
              auto  ptr = nd.fetch_tail(m_tail_ts);

                if(!ptr)
                {
                  break;
                }


              m_temporary_table.emplace_back(nd.get_observer(),*ptr);
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




}




