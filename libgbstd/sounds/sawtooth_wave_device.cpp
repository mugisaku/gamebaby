#include"libgbstd/sound.hpp"




namespace gbstd{




f32_t
sawtooth_wave_device::
get_sample() noexcept
{
  auto  v = m_sample_current                      ;
            m_sample_current += m_sample_increment;

  return v;
}


void
sawtooth_wave_device::
restart_phase() noexcept
{
    if(is_downward())
    {
      auto  n = get_number_of_samples_per_cycle();

      set_number_of_phase_samples(n);

      auto  v = get_volume();

      m_sample_current   = -v;
      m_sample_increment = (v*2)/n;
    }
}




}




