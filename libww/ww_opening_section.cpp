#include"libww/ww_opening_section.hpp"
#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




namespace{
const scene
g_scenes[] =
{
  {{0,80},"text00"},
  {{0,80},"text01"},
  {{0,80},"text02"},
  {{0,80},"text03"},
  {{0,80},"text04"},
  {{0,80},"text05"},
  {{0,80},"text06"},
  {{0,80},"text07"},
  {{0,80},"text08"},
};
}




opening_section::
opening_section(system&  sys) noexcept:
m_system(sys)
{
}


void
opening_section::
startup(gbstd::process&  proc) noexcept
{
  m_scene     = std::begin(g_scenes);
  m_scene_end = std::end(  g_scenes);

  get_clock_manager().add(m_clock.reset());

  live();
  show();
  set_interval(20);
  set_clock_watch(m_clock);

  get_task_manager().add(*this);

  advance();

  change(&opening_section::run);
}


void
opening_section::
cleanup(gbstd::process&  proc) noexcept
{
}




bool
opening_section::
advance() noexcept
{
    if(m_scene < m_scene_end)
    {
      auto&  scn = *m_scene++;

      auto  sv = m_system.get_text_section("opening")->get(scn.m_text_id);

      m_delay = m_clock.get_time()+(120*sv.size());

      m_text
        .clear()
        .push(sv)
      ;


      m_text_base_pos_y = scn.m_text_position_base.y;

      m_text.expose_all_characters();

      return true;
    }


  return false;
}


void
opening_section::
work() noexcept
{
}


void
opening_section::
render(const gbstd::canvas&  cv) noexcept
{
  auto  it     = m_text.begin();
  auto  it_end = m_text.end();

  int  y = m_text_base_pos_y;

  constexpr int  x_center = context::get_screen_width()/2;

    while(it != it_end)
    {
      auto&  ln = *it++;

      auto  sv = ln.get_view();

      int  w = gbstd::g_font_width*sv.size();

      cv.draw_string(gbstd::colors::white,sv,x_center-(w/2),y);

      y += gbstd::g_font_height;
    }
}


void
opening_section::
run(gbstd::process&  proc) noexcept
{
    if(gbstd::g_input.test_p())
    {
      proc.pop(1);

      return;
    }


    if(m_clock.get_time() >= m_delay)
    {
        if(!advance())
        {
          proc.pop(1);
        }
    }
}




}




