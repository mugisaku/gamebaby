#ifndef ww_context_hpp_is_included
#define ww_context_hpp_is_included


#include"libww/ww_front_section.hpp"




namespace ww{




class system;


class
context: public gbstd::section
{
  friend class system;

  system&  m_system;

  gbstd::clock  m_clock;

  static constexpr int  m_screen_width  = 288;
  static constexpr int  m_screen_height = 288;

  front_section    m_front_section;

  party  m_left_party;
  party  m_right_party;

  void  start_display_logo(gbstd::process&  proc) noexcept;
  void   wait_display_logo(gbstd::process&  proc) noexcept;

  void  startup(gbstd::process&  proc) noexcept override;

  context(system&  sys) noexcept;

public:
  static constexpr int  get_screen_width()  noexcept{return m_screen_width ;}
  static constexpr int  get_screen_height() noexcept{return m_screen_height;}

};


}




#endif




