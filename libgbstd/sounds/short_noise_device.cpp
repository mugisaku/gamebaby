#include"libgbstd/sound.hpp"




namespace gbstd{




void
short_noise_device::
update_seed() noexcept
{
  m_seed += m_seed + (((m_seed>>6)^
                       (m_seed>>5))&1);
}


}




