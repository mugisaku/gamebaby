#ifndef sdl_hpp_was_inclueded
#define sdl_hpp_was_inclueded


#include"libgbstd/image.hpp"
#include"libgbstd/control.hpp"
#include"libgbstd/sound.hpp"


namespace sdl{


void  update_screen(const gbstd::canvas&  cv) noexcept;

void  delay(uint32_t  ms) noexcept;

void  quit() noexcept;

void  init(                int  w, int  h, double  scale=1) noexcept;
void  init(int  x, int  y, int  w, int  h, double  scale=1) noexcept;

void  init_sound(int  sampling_rate) noexcept;
void  quit_sound() noexcept;


void  make_wave_table() noexcept;

void    set_keyon(int  i) noexcept;
void  unset_keyon(      ) noexcept;

void  start_sound_recording() noexcept;
void  clear_sound_recording() noexcept;
void   stop_sound_recording() noexcept;
bool   test_sound_recording() noexcept;

std::vector<uint8_t>  get_sound_wave_binary() noexcept;

void  add_sound(const char*  name, const char*  text) noexcept;

void  change_bgm(const char*  name) noexcept;
void  stop_bgm() noexcept;
void  play_sound(const char*  name) noexcept;

void  start_sound_playing() noexcept;
void   stop_sound_playing() noexcept;

void  resize_screen(int  w, int  h) noexcept;

void  screenshot() noexcept;

void  start_screen_recording() noexcept;
void    end_screen_recording() noexcept;
bool   test_screen_recording() noexcept;

gbstd::canvas  make_screen_canvas() noexcept;

void  update_control() noexcept;


}




#endif




