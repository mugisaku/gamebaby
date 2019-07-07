#include"libgbstd/sound.hpp"




namespace gbstd{




f32_t
sine_wave_device::
get_sample() noexcept
{
  auto  i = static_cast<int>(m_counter);

  auto  rate = g_cos_table[i];

  auto  v = get_volume()*rate;

  m_counter += m_increment;

  return v;
}


void
sine_wave_device::
restart_phase() noexcept
{
  auto  n = get_number_of_samples_per_cycle();

  set_number_of_phase_samples(n/2);

  m_increment = static_cast<f32_t>(g_cos_table_length-1)/(n/2);

    if(is_upward())
    {
      m_counter = 0;
    }

  else
    {
      m_counter   = g_cos_table_length-1;
      m_increment *=                  -1;
    }
}




}




