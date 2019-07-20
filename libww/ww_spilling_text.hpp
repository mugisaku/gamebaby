#ifndef ww_spilling_text_hpp_is_included
#define ww_spilling_text_hpp_is_included


#include"libgbstd/instruction_queue.hpp"
#include"libgbstd/process.hpp"


namespace ww{


class system;


class
spilling_text: public gbstd::task
{
  friend class system;

  system&  m_system;

  gbstd::task  m_task;

  gbstd::fixed_t  m_y_vector;

  gbstd::real_point  m_pos;
  gbstd::real_point  m_bottom_pos;

  gbstd::color  m_color;

  std::u16string  m_text;

  uint32_t  m_time;

  spilling_text(system&  sys) noexcept;

public:
  spilling_text&  set_text(std::u16string&&     text) noexcept{  m_text  = std::move(text);  return *this;}
  spilling_text&  set_text(std::u16string_view  text) noexcept{  m_text  =           text ;  return *this;}
  spilling_text&  set_color(gbstd::color    color) noexcept{  m_color =           color;  return *this;}
  spilling_text&  set_time(uint32_t          time) noexcept{  m_time  =            time;  return *this;}

  spilling_text&  reset(gbstd::clock&  clock, gbstd::point  bottom_pos, int  offset) noexcept;

  bool  is_finished() const noexcept{return m_text.empty();}

  void    work(                        ) noexcept override;
  void  render(const gbstd::canvas&  cv) noexcept override;
  void  finish(                        ) noexcept override;

};


}




#endif




