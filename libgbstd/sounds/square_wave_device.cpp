#include"libgbstd/sound.hpp"




namespace gbstd{




void
square_wave_device::
restart_phase() noexcept
{
  set_number_of_phase_samples(get_number_of_samples_per_cycle()/2);
}




}




