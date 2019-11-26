#ifndef GBSTD_VIDEO_hpp_was_included
#define GBSTD_VIDEO_hpp_was_included


#include<cstdint>
#include<cstdio>
#include<vector>
#include"libgbstd/image.hpp"
#include"libgbstd/misc.hpp"




namespace gbstd{


namespace gpfs{
class directory;
}


class
window_style
{
  int     m_frame_top_width=0;
  int  m_frame_bottom_width=0;

  int   m_frame_left_width=0;
  int  m_frame_right_width=0;

  color  m_fill_color;

public:
  window_style&  set_frame_top_width(   int  n) noexcept{  m_frame_top_width    = n;  return *this;}
  window_style&  set_frame_bottom_width(int  n) noexcept{  m_frame_bottom_width = n;  return *this;}
  window_style&  set_frame_left_width(  int  n) noexcept{  m_frame_left_width   = n;  return *this;}
  window_style&  set_frame_right_width( int  n) noexcept{  m_frame_right_width  = n;  return *this;}

  int  get_frame_top_width(   ) const noexcept{return m_frame_top_width   ;}
  int  get_frame_bottom_width() const noexcept{return m_frame_bottom_width;}
  int  get_frame_left_width(  ) const noexcept{return m_frame_left_width  ;}
  int  get_frame_right_width( ) const noexcept{return m_frame_right_width ;}

  color          get_fill_color(          ) const noexcept{return m_fill_color                     ;}
  window_style&  set_fill_color(color  col)       noexcept{       m_fill_color = col;  return *this;}

};


class
window
{
  struct flags{
    static constexpr int  open = 1;
    static constexpr int  show = 2;
  };

  status_value<int>  m_status;

  point  m_position;

  int  m_content_width =0;
  int  m_content_height=0;

  window_style  m_style;

public:
          window() noexcept{}
 virtual ~window(){}

  bool  is_opened() const noexcept{return m_status.test(flags::open);}
  bool  is_shown()  const noexcept{return m_status.test(flags::show);}

  window&  show() noexcept{  m_status.set(  flags::show);  return *this;}
  window&  hide() noexcept{  m_status.unset(flags::show);  return *this;}

  window&  set_x_position(int  n) noexcept{  m_position.x = n;  return *this;}
  window&  set_y_position(int  n) noexcept{  m_position.y = n;  return *this;}

  int  get_x_position() const noexcept{return m_position.x;}
  int  get_y_position() const noexcept{return m_position.y;}

  int  get_width()  const noexcept;
  int  get_height() const noexcept;

  int  get_content_width()  const noexcept{return m_content_width ;}
  int  get_content_height() const noexcept{return m_content_height;}

  window&  set_content_width( int  v) noexcept{  m_content_width  = v;  return *this;}
  window&  set_content_height(int  v) noexcept{  m_content_height = v;  return *this;}

  int  get_content_x_position() const noexcept{return m_position.x+get_style().get_frame_left_width();}
  int  get_content_y_position() const noexcept{return m_position.y+get_style().get_frame_top_width();}

  point  get_content_position() const noexcept{return point(get_content_x_position(),get_content_y_position());}

  canvas  get_content_canvas(const canvas&  base_cv) const noexcept;
  canvas      get_frame_top_canvas(const canvas&  base_cv) const noexcept;
  canvas     get_frame_left_canvas(const canvas&  base_cv) const noexcept;
  canvas    get_frame_right_canvas(const canvas&  base_cv) const noexcept;
  canvas   get_frame_bottom_canvas(const canvas&  base_cv) const noexcept;
  canvas  get_frame_content_canvas(const canvas&  base_cv) const noexcept;

        window_style&  get_style()       noexcept{return m_style;}
  const window_style&  get_style() const noexcept{return m_style;}

  virtual void  render_frame(const canvas&  cv) noexcept;

  virtual canvas  render(const canvas&  cv) noexcept;

};


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

gpfs::directory&  get_video_sprites_directory() noexcept;

void  redraw_video() noexcept;

const canvas&  set_video_size(int  w, int  h) noexcept;
const canvas&  get_video_canvas() noexcept;
const image&    get_video_image() noexcept;



}


#endif




