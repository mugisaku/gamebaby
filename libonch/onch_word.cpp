#include"libonch/onch.hpp"
#include"libgbstd/sound.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




onch_word&
onch_word::
set_v_level(int  start, int  end) noexcept
{
  m_start_v_level = start&0x1F;
  m_end_v_level   =   end&0x1F;

  return *this;
}


onch_word&
onch_word::
set_f_index(int  start, int  end) noexcept
{
  m_start_f_index = (            start-1)&0x7F;
  m_end_f_index   = ((!end? start:end)-1)&0x7F;

  return *this;
}




template<typename  T>
void
do_mix(uint32_t  rate, const sound_instruction&  instr, f32_t*  ptr) noexcept
{
  T  tmp(rate);

  tmp.mix(instr,ptr);
}


void
do_mix(gbstd::sound_kind  k, uint32_t  rate, const sound_instruction&  instr, f32_t*  ptr) noexcept
{
    switch(k)
    {
  case(sound_kind::sine_wave):
      do_mix<sine_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::square_wave):
      do_mix<square_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::triangle_wave):
      do_mix<triangle_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::sawtooth_wave):
      do_mix<sawtooth_wave_device>(rate,instr,ptr);
      break;
  case(sound_kind::noise):
      do_mix<gbstd::noise_device>(rate,instr,ptr);
      break;
  case(sound_kind::short_noise):
      do_mix<gbstd::short_noise_device>(rate,instr,ptr);
      break;
    }
}


void
onch_word::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
  auto  l = get_output_length();

  auto  num_samples = gbstd::sound_device::get_number_of_samples(ctx.m_sampling_rate,l);

    if(is_play() && num_samples)
    {
      std::vector<f32_t>  buf(num_samples);

      gbstd::sound_instruction  instr;

      constexpr double  v_base = 0.6/32;

      instr.set_length(l)
           .set_start_volume(v_base*m_start_v_level)
           .set_end_volume(  v_base*m_end_v_level)
           .set_start_frequency(gbstd::g_scale_table[m_start_f_index])
           .set_end_frequency(  gbstd::g_scale_table[  m_end_f_index]);

      do_mix(k,ctx.m_sampling_rate,instr,buf.data());

        if(!m_vibrato_amount)
        {
            for(int  i = 0;  i < num_samples;  ++i)
            {
              ctx.m_it[i] += buf[i];
            }
        }

      else
        {
          auto  f = 1<<(m_vibrato_amount&7);

          std::vector<f32_t>  subbuf(num_samples);

          constexpr double  v = 0.4;

          instr.set_length(l)
               .set_start_volume(v)
               .set_end_volume(  v)
               .set_start_frequency(f)
               .set_end_frequency(  f);

          do_mix(gbstd::sound_kind::sine_wave,ctx.m_sampling_rate,instr,subbuf.data());

            for(int  i = 0;  i < num_samples;  ++i)
            {
              auto  rate = (1.0+subbuf[i]);

              ctx.m_it[i] += buf[i]*rate;
            }
        }
    }


  ctx.m_it += num_samples;
}


void
onch_word::
print() const noexcept
{
  printf("word{");

    if(is_rest())
    {
      printf("r%u",get_output_length());
    }

  else
    {
        if(is_play())
        {
          printf("p%u",get_output_length());
        }


      printf(":v%d->%d",m_start_v_level,m_end_v_level);
      printf(":f%d->%d",m_start_f_index+1,m_end_f_index+1);

      printf(":b%d",m_vibrato_amount);
    }


  printf("}");
}


}




