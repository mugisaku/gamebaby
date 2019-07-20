#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{




view&
view::
set_table(table&  tbl) noexcept
{
  m_table = &tbl;

  set_width( tbl.get_width() );
  set_height(tbl.get_height());

  m_offset = {0,0};

  return *this;
}




view&
view::
set_x_offset(int  n) noexcept
{
  m_offset.x = std::max(0,std::min(n,m_table->get_width()-m_width));

  return *this;
}


view&
view::
set_y_offset(int  n) noexcept
{
  m_offset.y = std::max(0,std::min(n,m_table->get_height()-m_height));

  return *this;
}




void
view::
render(const gbstd::canvas&  cv) noexcept
{
  m_table->render(m_offset.x,m_offset.y,m_width,m_height,cv);
}




}}




