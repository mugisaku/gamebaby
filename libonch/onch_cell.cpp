#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




void
onch_cell::
print() const noexcept
{
    switch(m_kind)
    {
  case(sound_kind::square_wave):
      printf("square{");
      break;
  case(sound_kind::noise):
      printf("noise{");
      break;
  case(sound_kind::short_noise):
      printf("short_noise{");
      break;
    }


  onch_text::print();

  printf("}");
}




}




