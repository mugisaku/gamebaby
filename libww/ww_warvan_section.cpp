#include"libww/ww_warvan_section.hpp"
#include"libww/ww_context.hpp"
#include"libww/ww_system.hpp"




namespace ww{




namespace{
}


warvan_section::
warvan_section(system&  sys, gbstd::execution&  exec) noexcept:
m_system(sys)
{
  exec.get_equipped_clock().clear().set_permil(1000);

  m_task.set_data(this)
    .set_interval(20)
    .set_draw(draw)
    .set_tick(tick)
    .set_clock_watch(*exec.get_clock_by_id("standard"))
    .update_next_time()
  ;

  exec.add_task(m_task);

  exec.push_frame({
    {"*",run,*this},
  });
}


warvan_section::
~warvan_section()
{
}




void
warvan_section::
tick(this_type&  fr) noexcept
{
}


void
warvan_section::
draw(const gbstd::canvas&  cv, this_type&  fr) noexcept
{
}


void
warvan_section::
run(gbstd::execution&  exec, this_type&  fr) noexcept
{
report;
}




}




