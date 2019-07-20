#ifndef ww_system_hpp_is_included
#define ww_system_hpp_is_included


#include"libww/ww_spilling_text.hpp"
#include"libww/ww_context.hpp"


namespace ww{




class
system
{
  int  m_spilling_text_counter=0;

  gbstd::string_set  m_string_set;

public:
  system() noexcept;

  context&  create_context() noexcept{return *(new context(*this));}

  const gbstd::string_set::section*  get_text_section(std::string_view  sv) const noexcept;

  spilling_text&  create_spilling_text() noexcept;

  system&  collect_spilling_text(spilling_text&  ref) noexcept;

  int  get_spilling_text_counter() const noexcept{return m_spilling_text_counter;}

};




}




#endif




