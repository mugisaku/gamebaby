#ifndef gbstd_sound_hpp_is_included
#define gbstd_sound_hpp_is_included


#include"libgbstd/utility.hpp"


namespace gbstd{




using f32_t = float;




enum class
sound_kind
{
  null,
  sine_wave,
  square_wave,
  triangle_wave,
  sawtooth_wave,
  noise,
  short_noise,

};


class
sound_instruction
{
  uint32_t  m_length=0;

  f32_t  m_start_volume=0;
  f32_t  m_end_volume=0;

  f32_t  m_start_frequency=0;
  f32_t  m_end_frequency=0;

public:
  sound_instruction() noexcept{}

  sound_instruction&  set_length(uint32_t  l) noexcept{  m_length    = l;  return *this;}      

  sound_instruction&  set_start_volume(   f32_t  v) noexcept{  m_start_volume    = v;  return *this;}      
  sound_instruction&  set_end_volume(     f32_t  v) noexcept{  m_end_volume      = v;  return *this;}      
  sound_instruction&  set_start_frequency(f32_t  f) noexcept{  m_start_frequency = f;  return *this;}      
  sound_instruction&  set_end_frequency(  f32_t  f) noexcept{  m_end_frequency   = f;  return *this;}      

  uint32_t  get_length() const noexcept{return m_length;}

  f32_t  get_start_frequency() const noexcept{return m_start_frequency;}
  f32_t  get_end_frequency()   const noexcept{return m_end_frequency;}
  f32_t  get_start_volume()    const noexcept{return m_start_volume;}
  f32_t  get_end_volume()      const noexcept{return m_end_volume;}

};




//vm is Volume    Modulation音量変調
//fm is Frequency Modulation周波数変調
class
sound_device
{
  uint32_t  m_sampling_rate=0;

  uint32_t  m_number_of_phase_samples=0;

  f32_t  m_vm_current=0;
  f32_t  m_fm_current=0;

  bool  m_downward_flag=false;
  bool  m_dirty_flag=false;

protected:
  virtual f32_t  get_sample() noexcept{return is_downward()? -get_volume():get_volume();}

  void  set_number_of_phase_samples(uint32_t  v) noexcept{m_number_of_phase_samples = v;}

  virtual void  restart_phase() noexcept=0;

public:
  sound_device(uint32_t  sampling_rate=0) noexcept;

  void  reset() noexcept;

  sound_device&  set_sampling_rate(uint32_t  rate)       noexcept;
  uint32_t       get_sampling_rate(              ) const noexcept{return m_sampling_rate;}

  uint32_t  get_number_of_samples_per_cycle() const noexcept{return get_sampling_rate()/get_frequency();}

  uint32_t  get_number_of_phase_samples() const noexcept{return m_number_of_phase_samples;}

  bool  is_upward()   const noexcept{return !m_downward_flag;}
  bool  is_downward() const noexcept{return  m_downward_flag;}

  f32_t  get_volume()    const noexcept{return m_vm_current;}
  f32_t  get_frequency() const noexcept{return m_fm_current;}

  sound_device&  set_volume(f32_t     v) noexcept{  m_vm_current = v;  reset();  return *this;}
  sound_device&  set_frequency(f32_t  v) noexcept{  m_fm_current = v;  reset();  return *this;}

  f32_t  operator*() noexcept;

  void  mix(const sound_instruction&  instr, f32_t*  ptr) noexcept;

  static constexpr uint32_t  get_number_of_samples(uint32_t  sampling_rate, uint32_t  length) noexcept
  {return static_cast<double>(sampling_rate)/1000*length;}

};


class
sine_wave_device: public sound_device
{
  f32_t  m_counter;
  f32_t  m_increment;

  f32_t  get_sample() noexcept override;

  void  restart_phase() noexcept override;

public:
  using sound_device::sound_device;

};


class
square_wave_device: public sound_device
{
  void  restart_phase() noexcept override;

public:
  using sound_device::sound_device;

};


class
triangle_wave_device: public sound_device
{
protected:
  f32_t  m_sample_current;
  f32_t  m_sample_increment;

  f32_t  get_sample() noexcept override;

  void  restart_phase() noexcept override;

public:
  using sound_device::sound_device;

};


class
sawtooth_wave_device: public triangle_wave_device
{
  f32_t  get_sample() noexcept override;

  void  restart_phase() noexcept override;

public:
  using triangle_wave_device::triangle_wave_device;

};


class
noise_device: public sound_device
{
protected:
  uint16_t  m_seed=0xFFFF;

  virtual void  update_seed() noexcept;

  void  restart_phase() noexcept override;

  f32_t  get_sample() noexcept override{return static_cast<int16_t>(m_seed)/32767.0*get_volume();}

public:
  using sound_device::sound_device;

};


class
short_noise_device: public noise_device
{
  void  update_seed() noexcept override;

public:
  using noise_device::noise_device;

};


constexpr int  g_scale_table_length   = 128;
constexpr int  g_pitch_standard_index =  48;

extern const double  g_scale_table[g_scale_table_length];


}




#endif




