#include"libgbstd/process.hpp"




namespace gbstd{




task&
task::
reset() noexcept
{
  clear_status();

  m_name.clear();
  m_id.clear();

  m_interval  = 0;
  m_next_time = 0;

  m_last_time    = 0;
  m_elapsed_time = 0;
  m_living_time  = 0;
  m_die_count    = 0;
  m_getup_count  = 0;

  return *this;
}


task&
task::
die() noexcept
{
    if(is_living())
    {
      m_status.unset(flags::live);

      ++m_die_count;

        if(m_status.test(flags::discard_when_die))
        {
          discard();
        }
    }


  return *this;
}




task&
task::
sleep() noexcept
{
  m_status.set(  flags::sleep);
  m_status.unset(flags::alarm);

  return *this;
}


task&
task::
sleep(uint32_t  t) noexcept
{
  m_status.set(flags::sleep);
  m_status.set(flags::alarm);

  m_last_time = m_clock_watch.get_time();
  m_next_time = m_last_time+t;

  return *this;
}


task&
task::
getup() noexcept
{
    if(is_sleeping())
    {
      m_status.unset(flags::sleep);
      m_status.unset(flags::alarm);

      ++m_getup_count;

        if(m_status.test(flags::die_when_getup))
        {
          die();
        }

      else
        {
          update_next_time();
        }
    }


  return *this;
}


task&
task::
discard_when_die() noexcept
{
  m_status.set(flags::discard_when_die);

  return *this;
}


task&
task::
update_next_time() noexcept
{
  m_last_time = m_clock_watch.get_time();
  m_next_time = m_last_time+m_interval;

  return *this;
}


void
task::
operator()(const canvas*  cv) noexcept
{
  auto  now = m_clock_watch.get_time();

    if(is_living() && is_sleeping())
    {
        if(m_status.test(flags::alarm) && (now >= m_next_time))
        {
          getup();
        }
    }


    while(is_living() && !is_sleeping() && (now >= m_next_time))
    {
      work();

        if(!m_interval)
        {
          break;
        }


      m_next_time += m_interval;
    }



  auto  diff = now-m_last_time;

  m_elapsed_time += diff;

    if(is_living())
    {
      m_living_time += diff;
    }


  m_last_time = now;


    if(cv)
    {
      redraw(*cv);
    }
}


void
task::
redraw(const canvas&  cv) noexcept
{
    if(is_living())
    {
    }
}




}




