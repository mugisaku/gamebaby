#include"libgbstd/sns.hpp"
#include"libgbstd/utility.hpp"
#include<algorithm>



namespace gbstd{
namespace sns{




timeline&
timeline::
assign(timeline&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_nodes             ,rhs.m_nodes             );
      std::swap(m_top               ,rhs.m_top               );
      std::swap(m_bottom            ,rhs.m_bottom            );
      std::swap(m_number_of_articles,rhs.m_number_of_articles);
      std::swap(m_head_ts           ,rhs.m_head_ts           );
      std::swap(m_tail_ts           ,rhs.m_tail_ts           );
    }


  return *this;
}




void
timeline::
clear() noexcept
{
  auto  ptr = m_top;

    while(ptr)
    {
      auto  tmp = ptr->m_backward;

      delete ptr      ;
             ptr = tmp;
    }


  m_top    = nullptr;
  m_bottom = nullptr;

  m_number_of_articles = 0;
}




namespace{
auto  cmp =
[](const article*  a, const article*  b)->bool
{
  return a->get_timestamp() < b->get_timestamp();
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


  clear();
}


uint64_t
timeline::
read_forward() noexcept
{
  uint64_t  n = 0;

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


              m_temporary_table.emplace_back(new article(nd.get_observer(),*recptr));

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
            n = m_temporary_table.size();

      std::stable_sort(p,p+n,cmp);

      auto      it = m_temporary_table.begin();
      auto  end_it = m_temporary_table.end();

        while(it != end_it)
        {
          push_forward(*it++);
        }
    }


  return n;
}


uint64_t
timeline::
read_backward() noexcept
{
  uint64_t  n = 0;

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


              m_temporary_table.emplace_back(new article(nd.get_observer(),*recptr));

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
            n = m_temporary_table.size();

      std::stable_sort(p,p+n,cmp);

      auto      it = m_temporary_table.rbegin();
      auto  end_it = m_temporary_table.rend();

        while(it != end_it)
        {
          push_backward(*it++);
        }
    }


  return n;
}


void
timeline::
push_forward(article*  a) noexcept
{
  article::link(a,m_top);

  m_top = a;

    if(!m_bottom)
    {
      m_bottom = a;
    }


  ++m_number_of_articles;
}


void
timeline::
push_backward(article*  a) noexcept
{
  article::link(m_bottom,a);

  m_bottom = a;

    if(!m_top)
    {
      m_top = a;
    }


  ++m_number_of_articles;
}




timeline::iterator
timeline::iterator::
operator+(int  n) const noexcept
{
  auto  p = m_pointer;

       if(n > 0){while(n--){p = p->m_forward ;}}
  else if(n < 0){while(n++){p = p->m_backward;}}

  return p;
}


timeline::iterator
timeline::iterator::
operator-(int  n) const noexcept
{
  auto  p = m_pointer;

       if(n > 0){while(n--){p = p->m_backward;}}
  else if(n < 0){while(n++){p = p->m_forward ;}}

  return p;
}




timeline::reverse_iterator
timeline::reverse_iterator::
operator+(int  n) const noexcept
{
  auto  p = m_pointer;

       if(n > 0){while(n--){p = p->m_backward;}}
  else if(n < 0){while(n++){p = p->m_forward ;}}

  return p;
}


timeline::reverse_iterator
timeline::reverse_iterator::
operator-(int  n) const noexcept
{
  auto  p = m_pointer;

       if(n > 0){while(n--){p = p->m_forward ;}}
  else if(n < 0){while(n++){p = p->m_backward;}}

  return p;
}




}}




