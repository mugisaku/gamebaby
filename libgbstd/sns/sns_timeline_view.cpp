#include"libgbstd/sns.hpp"
#include"libgbstd/utility.hpp"
#include<algorithm>



namespace gbstd{
namespace sns{




timeline_view&
timeline_view::
assign(const timeline&  tl) noexcept
{
  m_timeline = &tl;

  reset();


  return *this;
}


int
timeline_view::
get_number_of_articles() const noexcept
{
  int  n = 0;

  auto      it = begin();
  auto  end_it =   end();

    while(it != end_it)
    {
       ++n;
      ++it;
    }


  return n;
}


timeline_view&
timeline_view::
reset() noexcept
{
    if(m_timeline)
    {
               m_head = m_timeline->rbegin().to_pointer();
      m_tail = m_head                                    ;
    }

  else
    {
      m_head = nullptr;
      m_tail = nullptr;
    }


  m_distance = 0;

  return *this;
}


int
timeline_view::
set_distance(int  n) noexcept
{
    if(m_head)
    {
      m_distance = 0;

      m_tail = m_head;

        while(n--)
        {
          auto  backward = m_tail->m_backward;

            if(!backward)
            {
              break;
            }


          m_tail = backward;

          ++m_distance;
        }

    }


  return m_distance;
}


void
timeline_view::
move_forward( int  n) noexcept
{
}


void
timeline_view::
move_backward(int  n) noexcept
{
}




}}




