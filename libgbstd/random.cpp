#include"libgbstd/random.hpp"




namespace gbstd{




void
initialize_random() noexcept
{
  std::random_device  g_random_device;

  g_random_number_engine.seed(g_random_device());
}


std::default_random_engine
g_random_number_engine;




}




