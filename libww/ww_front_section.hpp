#ifndef ww_front_section_hpp_is_included
#define ww_front_section_hpp_is_included


#include"libww/ww_character.hpp"
#include"libww/ww_bar.hpp"
#include"libww/ww_spilling_text.hpp"
#include"libww/ww_party.hpp"
#include"libww/ww_opening_section.hpp"
#include<list>
#include<vector>




namespace ww{



class system;


class
front_section: public gbstd::section
{
  friend class system;

  system&  m_system;

  gbstd::clock  m_clock;
  gbstd::timer  m_timer;

  int  m_phase;

  opening_section  m_opening_section;

  void  run(gbstd::process&  proc) noexcept;

  void  render(const gbstd::canvas&  cv) noexcept;

  void  startup(gbstd::process&  proc) noexcept override;
  void  cleanup(gbstd::process&  proc) noexcept override;

public:
  front_section(system&  sys) noexcept;

};


}




#endif




