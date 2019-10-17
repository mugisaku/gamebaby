#ifndef GBSTD_SCREEN_hpp_was_included
#define GBSTD_SCREEN_hpp_was_included


#include<cstdint>
#include<cstdio>
#include<vector>
#include"libgbstd/image.hpp"
#include"libgbstd/misc.hpp"


namespace gbstd{




class
sprite
{
  status_value<int>  m_status;

  struct flags{
    static constexpr int       hide = 1;
    static constexpr int      blink = 2;
    static constexpr int  blink_bit = 4;

  };


  uint32_t  m_blink_show_value=0;
  uint32_t  m_blink_hide_value=0;
  uint32_t  m_blink_counter=0;

  callback_wrapper  m_callback;

public:
  sprite&  show() noexcept{  m_status.unset(flags::hide);  return *this;}
  sprite&  hide() noexcept{  m_status.set(  flags::hide);  return *this;}

  sprite&    blink() noexcept{  m_status.set(  flags::blink);  return *this;}
  sprite&  unblink() noexcept{  m_status.unset(flags::blink);  return *this;}

  sprite&  set_blinking_rate(int  show, int  hide) noexcept;

  bool  is_blinking() const noexcept{return m_status.test(flags::blink);}
  bool  is_hiding()   const noexcept{return m_status.test(flags::hide);}

  sprite&  set_callback(callback_wrapper  cb) noexcept{  m_callback = cb;   return *this;}

  sprite&  operator()(const canvas&  cv) noexcept;

};


void    set_redraw_flag() noexcept;
void  unset_redraw_flag() noexcept;
bool   test_redraw_flag() noexcept;

const color&  get_background_color(               ) noexcept;
void          set_background_color(color  bg_color) noexcept;

void  allocate_sprites(int  n) noexcept;
sprite&  get_sprite(int  i) noexcept;

void  redraw_screen() noexcept;

const canvas&  set_screen_size(int  w, int  h) noexcept;
const canvas&  get_screen_canvas() noexcept;
const image&    get_screen_image() noexcept;



}


#endif



