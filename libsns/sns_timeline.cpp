#include"libsns/sns.hpp"
#include<algorithm>




namespace sns{




timeline::
target::
target(const account_observer&  obs) noexcept
{
    if(obs)
    {
      auto&  tbl = obs->get_article_table();

        if(tbl.get_number_of_records())
        {
          m_observer = obs;

          m_bottom_index = tbl.get_last().get_index();
        }
    }
}




void
timeline::
reset(timestamp  base) noexcept
{
  m_article_table.clear();

  m_front_timestamp = base;
  m_back_timestamp  = base;
}


void
timeline::
fetch_front_articles(timestamp   ts) noexcept
{
  std::vector<const article*>  ls;

    for(auto&  t: m_target_list)
    {
        if(t.m_observer)
        {
          auto&  acc = *t.m_observer;

            for(auto&  art: acc.get_article_table())
            {
                if(art <= m_front_timestamp)
                {
                  break;
                }


              ls.emplace_back(&art);
            }
        }
    }


  constexpr auto  cmp = [](const article* const&  a, const article* const&  b) noexcept->bool{
    return a->get_timestamp() < b->get_timestamp();
  };


  std::stable_sort(ls.begin(),ls.end(),cmp);

  m_front_timestamp = ts;

    for(auto  ptr: ls)
    {
      m_article_table.emplace_front(ptr);
    }
}


void
timeline::
fetch_back_articles() noexcept
{
    for(auto&  t: m_target_list)
    {
        if(t.m_observer)
        {
          auto&  tbl = t.m_observer->get_article_table();

          auto&  art = tbl[t.m_bottom_index];

            if(art < m_back_timestamp)
            {
              m_back_timestamp = art.get_timestamp();
            }
        }
    }


  fetch_back_articles(m_back_timestamp);
}


void
timeline::
fetch_back_articles(timestamp   ts) noexcept
{
  std::vector<const article*>  ls;

    for(auto&  t: m_target_list)
    {
        if(t.m_observer)
        {
          auto&  tbl = t.m_observer->get_article_table();

            for(;;)
            {
              auto&  i = t.m_bottom_index;

              auto&  art = tbl[i];

                if(art < ts)
                {
                  ls.emplace_back(&art);

                    if(!i)
                    {
                      t.m_observer.unrefer();
                    }

                  else
                    {
                      --i;
                    }
                }
            }
        }
    }


  m_back_timestamp = ts;

  constexpr auto  cmp = [](const article* const&  a, const article* const&  b) noexcept->bool{
    return a->get_timestamp() > b->get_timestamp();
  };


  std::stable_sort(ls.rbegin(),ls.rend(),cmp);

    for(auto  ptr: ls)
    {
      m_article_table.emplace_back(ptr);
    }
}




}




