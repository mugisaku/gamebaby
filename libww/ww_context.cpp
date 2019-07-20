#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




context::
context(system&  sys) noexcept:
m_system(sys),
m_front_section(sys)
{
}




void
context::
startup(gbstd::process&  proc) noexcept
{
  m_clock.set_name("standard")
    .set_id("standard")
    .set_permil(1000)
    .turnon();

  proc.get_clock_manager().add(m_clock);

  change(&context::start_display_logo);
}




void
context::
start_display_logo(gbstd::process&  proc) noexcept
{
  auto&  spl = m_system.create_spilling_text();

  std::u16string_view  sv = u"Game Baby";

  auto  text_width = static_cast<int>(gbstd::g_font_width*sv.size());

  int  x_center = m_screen_width/2;
  int  y_center = m_screen_height/2;

  spl.set_text(sv)
    .set_color(gbstd::colors::yellow)
    .set_time(600)
    .reset(m_clock,{x_center-(text_width/2),y_center-8},-32);

  get_task_manager().add(spl);

  change(&context::wait_display_logo);
}


void
context::
wait_display_logo(gbstd::process&  proc) noexcept
{
    if(!m_system.get_spilling_text_counter())
    {
      proc.push(m_front_section);
    }


  proc.interrupt();
}



}




