#include"libgbstd/sound.hpp"




namespace gbstd{




namespace{
void
set_modulation(f32_t  start, f32_t  end, uint32_t  ms, f32_t&  cur, f32_t&  inc) noexcept
{
    if(ms <= 1)
    {
      cur = end;
      inc =   0;

      return;
    }


  auto  diff = std::abs(end-start);

  cur = start;
  inc = diff/ms;

    if(start > end)
    {
      inc = -inc;
    }
}
}




sound_device::
sound_device(uint32_t  sampling_rate) noexcept
{
  set_sampling_rate(sampling_rate);
}




void
sound_device::
reset() noexcept
{
  m_dirty_flag = true;
}


sound_device&
sound_device::
set_sampling_rate(uint32_t  rate) noexcept
{
  m_sampling_rate = rate;

  reset();

  return *this;
}


f32_t
sound_device::
operator*() noexcept
{
    if(m_dirty_flag)
    {
      m_downward_flag = false;
      m_dirty_flag    = false;

      restart_phase();
    }


  uint32_t  safe_counter = 0;

    while(!m_number_of_phase_samples)
    {
        if(++safe_counter > 4)
        {
          break;
        }


      m_downward_flag = !m_downward_flag;

      restart_phase();
    }


  --m_number_of_phase_samples;

  return get_sample();
}


void
sound_device::
mix(const sound_instruction&  instr, f32_t*  ptr) noexcept
{
  auto  length = instr.get_length();

  auto  number_of_whole_samples = get_number_of_samples(m_sampling_rate,length);

  f32_t  fm_increment=0;
  f32_t  vm_increment=0;

  set_modulation(instr.get_start_volume()   ,instr.get_end_volume()   ,length,m_vm_current,vm_increment);
  set_modulation(instr.get_start_frequency(),instr.get_end_frequency(),length,m_fm_current,fm_increment);


  uint32_t  modulation_counter_base = get_sampling_rate()/1000;
  uint32_t  modulation_counter      = modulation_counter_base;

  m_downward_flag = false;

  restart_phase();

    while(number_of_whole_samples--)
    {
        if(!modulation_counter)
        {
          modulation_counter = modulation_counter_base;

          m_vm_current += vm_increment;
          m_fm_current += fm_increment;

               if(m_vm_current < 0.0){m_vm_current = 0.0;}
          else if(m_vm_current > 1.0){m_vm_current = 1.0;}

            if(m_fm_current < 0.0)
            {
              m_fm_current = 2.0;
            }
        }

      else
        {
          --modulation_counter;
        }


      *ptr++ = **this;
    }
}




}




