#include"libgbstd/process.hpp"




namespace gbstd{




void
process::
update_clocks(uint32_t  diff) noexcept
{
    for(auto&  ptr: m_clock_list)
    {
      ptr->add(diff);
    }


    if(m_current_section)
    {
        for(auto  ptr: m_current_section->m_clock_list)
        {
          ptr->add(diff);
        }
    }
}


template<typename  T>
void
process_tasks_internal(T&  ls, const canvas*  cv) noexcept
{
  auto  it = ls.begin();

    while(it != ls.end())
    {
      auto&  tsk = **it;

        if(tsk.is_discarded())
        {
          tsk.finish();

          it = ls.erase(it);
        }

      else
        {
          tsk(cv);

          ++it;
        }
    }
}


void
process::
process_tasks(const canvas*  cv) noexcept
{
  process_tasks_internal(m_task_list,cv);

    if(m_current_section)
    {
      process_tasks_internal(m_current_section->m_task_list,cv);
    }
}


void
process::
process_timers(uint32_t  diff) noexcept
{
    for(auto  ptr: m_timer_list)
    {
      (*ptr)(diff);
    }


    if(m_current_section)
    {
        for(auto  ptr: m_current_section->m_timer_list)
        {
          (*ptr)(diff);
        }
    }
}




process&
process::
push(section&  sec) noexcept
{
  m_section_stack.emplace_back(m_current_section);

  m_current_section = &sec;

  sec.startup(*this);

  return *this;
}


process&
process::
pop(int  value) noexcept
{
  m_current_section->cleanup(*this);

  m_current_section->m_task_list.clear();
  m_current_section->m_clock_list.clear();
  m_current_section->m_timer_list.clear();

  m_current_section = m_section_stack.back();

  m_section_stack.pop_back();

  m_current_section->m_return_value = value;

  m_current_section->update_next_time();

  return *this;
}




bool
process::
operator()() noexcept
{
    if(m_status.test(flags::busy))
    {
      return false;
    }


  m_status.set(flags::busy);

  auto  diff = g_time-m_last_time         ;
                      m_last_time = g_time;

  update_clocks(diff);
  process_timers(diff);

  int  m_safe_counter = 32;

    while(--m_safe_counter && *this)
    {
      auto&  sec = *m_current_section;

        if(sec)
        {
          sec(*this);

            if(m_status.test(flags::interrupt))
            {
              break;
            }
        }

      else
        {
          break;
        }
    }


  m_status.unset(flags::interrupt);

    if(g_time >= m_next_time)
    {
      m_canvas.fill(m_background_color);

      process_tasks(&m_canvas);

      m_next_time = g_time+m_interval;

      m_status.unset(flags::busy);

      return true;
    }


  process_tasks(nullptr);

  m_status.unset(flags::busy);

  return false;
}


}




