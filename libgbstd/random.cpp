#include"libgbstd/random.hpp"




namespace gbstd{




std::random_device
g_random_device;


std::default_random_engine
g_random_number_engine(g_random_device());




}




