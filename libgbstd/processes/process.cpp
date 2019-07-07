#include"libgbstd/process.hpp"




namespace gbstd{




process&
process::
assign(std::string_view  name, execution_entry  start) noexcept
{
  reset();

  m_name = name;

  push_frame({start},"initial frame");

  return *this;
}




void
process::
update_clocks() noexcept
{
  auto  diff = g_time-m_last_time         ;
                      m_last_time = g_time;

    for(auto&  ptr: m_clock_list)
    {
      ptr->add(diff);
    }


  m_equipped_clock.add(diff);
}


void
process::
process_tasks(const canvas*  cv) noexcept
{
  auto  it = m_task_list.begin();

    while(it != m_task_list.end())
    {
      auto&  tsk = **it;

        if(tsk.is_discarded())
        {
          tsk.finish();

          it = m_task_list.erase(it);
        }

      else
        {
          tsk();

            if(cv)
            {
              tsk(*cv);
            }


          ++it;
        }
    }
}




bool
process::
operator()() noexcept
{
  update_clocks();

  int  counter = 8;

    while(*this)
    {
      m_status.unset(flags::pc_barrier);

        if(m_pc < m_sp)
        {
          auto&  ent = *reinterpret_cast<const execution_entry*>(&m_memory[m_pc]);

          auto    cb = ent.get_callback();
          auto  data = static_cast<dummy*>(ent.get_data());

            if(test_verbose_flag())
            {
              print();
              printf("\n*calback:\"%s\" invoked\n\n",ent.get_name());
            }


          cb(*this,*data);

            if(!--counter || ent.test_interruption())
            {
              break;
            }
        }

      else
        {
          pop_frame();
        }
    }


    if(g_time >= m_next_time)
    {
      m_canvas.fill(m_background_color);

      process_tasks(&m_canvas);

      m_next_time = g_time+m_interval;

      return true;
    }


  process_tasks(nullptr);

  return false;
}


}




