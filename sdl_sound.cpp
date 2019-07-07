#include"sdl.hpp"
#include<SDL.h>
#include<unordered_map>
#include<list>
#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/sound.hpp"
#include"libgbstd/file_op.hpp"


namespace sdl{


namespace{


int
g_sampling_rate;


int
g_reverb_rate;


struct
reverb
{
  const gbstd::f32_t*  m_begin;
  const gbstd::f32_t*  m_current;
  const gbstd::f32_t*  m_end;

  int  m_remain;

  void  write(gbstd::f32_t*  dst, int  n) noexcept
  {
      while(n--)
      {
          if(!m_remain)
          {
            break;
          }


        --m_remain;

          if(m_current >= m_end)
          {
            m_current = m_begin;
          }


        auto  s = *m_current++;

        *dst++ += s*(static_cast<double>(m_remain)/g_reverb_rate);
      }
  }

};


struct
entry
{
  const gbstd::f32_t*  m_begin;
  const gbstd::f32_t*  m_current;
  const gbstd::f32_t*  m_end;

  entry(const gbstd::f32_t*  begin=nullptr, const gbstd::f32_t*  end=nullptr) noexcept:
  m_begin(begin), m_current(begin), m_end(end){}

  operator bool() const noexcept{return m_current;}

  void  write(gbstd::f32_t*  dst, int  n, bool  repeat) noexcept
  {
      if(*this)
      {
          while(n--)
          {
              if(m_current >= m_end)
              {
                  if(repeat)
                  {
                    m_current = m_begin;
                  }

                else
                  {
                    m_current = nullptr;

                    break;
                  }
              }


            *dst++ += *m_current++;
          }
      }
  }

};


entry
g_bgm_entry;


std::list<entry>
g_entry_list;


std::list<reverb>
g_reverb_list;


std::vector<gbstd::f32_t>
g_bgm_sound;


std::vector<gbstd::f32_t>
g_wave_table[gbstd::g_scale_table_length];


const gbstd::f32_t*  g_wave_begin;
const gbstd::f32_t*  g_wave_ptr;
const gbstd::f32_t*  g_wave_end;


std::unordered_map<std::string,std::vector<gbstd::f32_t>>
g_sound_map;


bool  g_recording_flag;

std::vector<int16_t>  g_recording_buffer;


SDL_AudioSpec  g_spec;

SDL_AudioDeviceID  g_devid;


void
callback(void*  data, uint8_t*  buf, int  len) noexcept
{
  SDL_memset(buf,g_spec.silence,len);

  auto  dst = reinterpret_cast<gbstd::f32_t*>(buf);

  int  n = len/sizeof(*dst);

  g_bgm_entry.write(dst,n,true);

    if(g_wave_begin)
    {
        for(int  i = 0;  i < n;  ++i)
        {
            if(g_wave_ptr >= g_wave_end)
            {
              g_wave_ptr = g_wave_begin;
            }


          dst[i] += *g_wave_ptr++;
        }
    }


  {
    auto  it = g_reverb_list.begin();

      while(it != g_reverb_list.end())
      {
        it->write(dst,n);

          if(it->m_remain)
          {
            ++it;
          }

        else
          {
            it = g_reverb_list.erase(it);
          }
      }
  }


  {
    auto  it = g_entry_list.begin();

      while(it != g_entry_list.end())
      {
        it->write(dst,n,false);

          if(*it)
          {
            ++it;
          }

        else
          {
            it = g_entry_list.erase(it);
          }
      }
  }
}


void
print() noexcept
{
  printf("freq %d\n",g_spec.freq);
  printf("ch %d\n",g_spec.channels);
  printf("smpl %d\n",g_spec.samples);

  const char*  s = "unknown";

    switch(g_spec.format)
    {
  case(AUDIO_U8    ): s = "U8";break;
  case(AUDIO_S8    ): s = "S8";break;
  case(AUDIO_S16LSB): s = "S16LSB";break;
  case(AUDIO_S16MSB): s = "S16MSB";break;
  case(AUDIO_U16LSB): s = "U16LSB";break;
  case(AUDIO_S32LSB): s = "S32LSB";break;
  case(AUDIO_S32MSB): s = "S32MSB";break;
  case(AUDIO_F32LSB): s = "F32LSB";break;
  case(AUDIO_F32MSB): s = "F32MSB";break;
    }


  printf("format %s\n",s);
}


}


void
init_sound(int  sampling_rate) noexcept
{
    if(SDL_WasInit(SDL_INIT_AUDIO))
    {
      return;
    }


  SDL_InitSubSystem(SDL_INIT_AUDIO);

  SDL_AudioSpec  spec;

  g_sampling_rate = sampling_rate;
  g_reverb_rate   = sampling_rate/2;

  spec.freq     = g_sampling_rate;
  spec.channels =               1;
  spec.format   =       AUDIO_F32;
  spec.samples  =            4096;
  spec.callback =        callback;

  g_devid = SDL_OpenAudioDevice(nullptr,0,&spec,&g_spec,0);

    if(g_devid == 0)
    {
      printf("%s\n",SDL_GetError());

      report;
    }
}


void
quit_sound() noexcept
{
    if(SDL_WasInit(SDL_INIT_AUDIO))
    {
      SDL_CloseAudioDevice(g_devid);

      SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}




void
make_wave_table() noexcept
{
    for(int  i = 0;  i < gbstd::g_scale_table_length;  ++i)
    {
      auto  f = gbstd::g_scale_table[i];

        if((g_sampling_rate/f) < 2)
        {
          break;
        }


      gbstd::triangle_wave_device  dev(g_sampling_rate);

      dev.set_volume(0.05).set_frequency(f);

      auto  num_sample = dev.get_number_of_samples_per_cycle();

      g_wave_table[i].resize(num_sample);

      auto  dst = g_wave_table[i].begin();

        while(num_sample--)
        {
          *dst++ = *dev;
        }
    }
}


void
set_keyon(int  i) noexcept
{
    if(i < gbstd::g_scale_table_length)
    {
      auto&  v = g_wave_table[i];

        if(v.data() != g_wave_begin)
        {
          SDL_LockAudioDevice(g_devid);

          g_wave_begin = v.data();
          g_wave_ptr   = v.data();
          g_wave_end   = v.data()+v.size();

          SDL_UnlockAudioDevice(g_devid);
        }
    }
}


void
unset_keyon() noexcept
{
    if(g_wave_begin)
    {
      SDL_LockAudioDevice(g_devid);

      reverb  rev;

      rev.m_begin   = g_wave_begin;
      rev.m_current =   g_wave_ptr;
      rev.m_end     =   g_wave_end;
      rev.m_remain  = g_reverb_rate;

      g_reverb_list.emplace_back(rev);

      g_wave_begin = nullptr;

      SDL_UnlockAudioDevice(g_devid);
    }
}


void
change_bgm(const char*  name) noexcept
{
  SDL_LockAudioDevice(g_devid);

  auto  it = g_sound_map.find(name);

    if(it != g_sound_map.cend())
    {
      g_bgm_entry.m_begin   = it->second.data();
      g_bgm_entry.m_current = it->second.data();
      g_bgm_entry.m_end     = it->second.data()+it->second.size();
    }


  SDL_UnlockAudioDevice(g_devid);
}


void
stop_bgm() noexcept
{
  SDL_LockAudioDevice(g_devid);

  g_bgm_entry.m_current = nullptr;

  SDL_UnlockAudioDevice(g_devid);
}


void
play_sound(const char*  name) noexcept
{
  SDL_LockAudioDevice(g_devid);

  auto  it = g_sound_map.find(name);

    if(it != g_sound_map.cend())
    {
      g_entry_list.emplace_back(it->second.data(),it->second.data()+it->second.size());
    }


  SDL_UnlockAudioDevice(g_devid);
}


void
add_sound(const char*  name, const char*  text) noexcept
{
/*
  gbstd::onch_space  sp;

  sp.load_from_string(text);

  g_sound_map.emplace(name,sp.make_f32_raw_binary(g_sampling_rate,0.05));
*/
}


void
start_sound_recording() noexcept
{
  g_recording_flag = true;
}


void
clear_sound_recording() noexcept
{
  g_recording_buffer.clear();
}


void
stop_sound_recording() noexcept
{
  g_recording_flag = false;
}


bool
test_sound_recording() noexcept
{
  return g_recording_flag;
}


std::vector<uint8_t>
get_sound_wave_binary() noexcept
{
  gbstd::wave_format  fmt;

  fmt.set_number_of_channels(1);
  fmt.set_sampling_rate(g_sampling_rate);
  fmt.set_number_of_bits_per_sample(16);

  fmt.update();

  gbstd::wave  wav;

  bool  playing = (SDL_GetAudioDeviceStatus(g_devid) == SDL_AUDIO_PLAYING);

    if(playing)
    {
      stop_sound_playing();
    }


  wav.assign(g_recording_buffer.data(),g_recording_buffer.size(),fmt);

    if(playing)
    {
      start_sound_playing();
    }


  return wav.to_binary();
}




void
start_sound_playing() noexcept
{
  SDL_PauseAudioDevice(g_devid,0);
}


void
stop_sound_playing() noexcept
{
  SDL_PauseAudioDevice(g_devid,1);
}




}




