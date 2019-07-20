#include"libww/ww_front_section.hpp"
#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




namespace{


const uint8_t
g_png_data[] =
{
#include"title.txt"
};


gbstd::image
g_image(g_png_data);
}


front_section::
front_section(system&  sys) noexcept:
m_system(sys),
m_opening_section(sys)
{
}




void
front_section::
startup(gbstd::process&  proc) noexcept
{
  get_clock_manager().add(m_clock.reset());
  get_timer_manager().add(m_timer.reset());

  set_interval(20);
  set_clock_watch(m_clock);

  get_task_manager().add(*this);

  m_phase = 0;

  change(&front_section::run);
}


void
front_section::
cleanup(gbstd::process&  proc) noexcept
{
}




void
front_section::
render(const gbstd::canvas&  cv) noexcept
{
    if(m_phase == 2)
    {
      gbstd::canvas  src_cv0(g_image,{0,0,48*5,48});
      gbstd::canvas  src_cv1(g_image,{0,48,48,48});
      gbstd::canvas  src_cv2(g_image,{48,48,48*3,24});
      gbstd::canvas  src_cv3(g_image,{48,48+24,24,24});

      cv.draw_canvas(src_cv0,20,40);
      cv.draw_canvas(src_cv1,0,20);
      cv.draw_canvas(src_cv2,70,88);
      cv.draw_canvas(src_cv3,40,10);
    }
}


void
front_section::
run(gbstd::process&  proc) noexcept
{
    if(m_phase == 0)
    {
      proc.push(m_opening_section);

      m_phase = 1;
    }

  else
    if(m_phase == 1)
    {
      m_timer.reset(3000);

      show();
      live();

      m_phase = 2;
    }

  else
    if(m_phase == 2)
    {
        if(m_timer)
        {
          m_phase = 0;
        }
    }
}




}




