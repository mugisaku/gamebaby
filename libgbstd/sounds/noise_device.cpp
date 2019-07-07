#include"libgbstd/sound.hpp"




namespace gbstd{




void
noise_device::
restart_phase() noexcept
{
  set_number_of_phase_samples(get_number_of_samples_per_cycle()/8/2);

    if(is_upward())
    {
      update_seed();
    }
}


void
noise_device::
update_seed() noexcept
{
  m_seed += m_seed + (((m_seed>>14)^
                       (m_seed>>13))&1);
}




}




