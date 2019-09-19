#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




menu_handler::
menu_handler(operating_node&  root) noexcept:
m_x_offset_label(root.create_label(u"   1/   1")),
m_y_offset_label(root.create_label(u"   1/   1"))
{
}




void
menu_handler::
process_before_reform() noexcept
{
  set_content_width( m_view->get_table().get_cell_width() *m_view->get_width() );
  set_content_height(m_view->get_table().get_cell_height()*m_view->get_height());
}




menu_handler&
menu_handler::
set_view(menus::view&  v) noexcept
{
  m_view = &v;

  m_seeking_pos = {};
  m_current_pos = {};

  request_reform();

  return *this;
}


void
menu_handler::
on_mouse_act(point  mouse_pos) noexcept
{
  int  x = mouse_pos.x/m_view->get_table().get_cell_width() ;
  int  y = mouse_pos.y/m_view->get_table().get_cell_height();

  point  current(x,y);

    if(m_seeking_pos != current)
    {
      call(menu_event::kind::leave);

      m_seeking_pos = current;

      call(menu_event::kind::enter);

      request_redraw();
    }


    if(get_modified_keys().test_mol())
    {
      m_current_pos = current;

        if(get_keys().test_mol())
        {
          call(menu_event::kind::press);
        }

      else
        {
          call(menu_event::kind::release);
        }


      request_redraw();
    }
}


void
menu_handler::
render(const canvas&  cv) noexcept
{
  render_background(cv);

  m_view->render(cv);

  int  w = m_view->get_table().get_cell_width() ;
  int  h = m_view->get_table().get_cell_height();

  cv.draw_rectangle(colors::black,w*m_seeking_pos.x  ,h*m_seeking_pos.y,w,h);
  cv.draw_rectangle(colors::white,w*m_current_pos.x+1,h*m_current_pos.y+1,w-2,h-2);
}




void
menu_handler::
update_x_offset_label() noexcept
{
  string_form  sf;

  int  x = m_view->get_x_offset();
  int  w = m_view->get_table().get_width()-m_view->get_width();

  m_x_offset_label.set_string_without_reform(sf("%4d/%4d",x+1,w+1));
}


void
menu_handler::
update_y_offset_label() noexcept
{
  string_form  sf;

  int  y = m_view->get_y_offset();
  int  h = m_view->get_table().get_height()-m_view->get_height();

  m_y_offset_label.set_string_without_reform(sf("%4d/%4d",y+1,h+1));
}


void
menu_handler::
up(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  mnu = evt->get_userdata<menu_handler>();

      mnu.m_view->add_y_offset(-1);

      mnu.update_y_offset_label();

      mnu.request_redraw();
    }
}
void
menu_handler::
left(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  mnu = evt->get_userdata<menu_handler>();

      mnu.m_view->add_x_offset(-1);

      mnu.update_x_offset_label();

      mnu.request_redraw();
    }
}
void
menu_handler::
right(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  mnu = evt->get_userdata<menu_handler>();

      mnu.m_view->add_x_offset(1);

      mnu.update_x_offset_label();

      mnu.request_redraw();
    }
}
void
menu_handler::
down(button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  mnu = evt->get_userdata<menu_handler>();

      mnu.m_view->add_y_offset(1);

      mnu.update_y_offset_label();

      mnu.request_redraw();
    }
}


node&
menu_handler::
create_up_button(operating_node&  root) noexcept
{
  return root.create_button(icons::up).set_callback(up).set_userdata(*this);
}


node&
menu_handler::
create_left_button(operating_node&  root) noexcept
{
  return root.create_button(icons::left).set_callback(left).set_userdata(*this);
}


node&
menu_handler::
create_right_button(operating_node&  root) noexcept
{
  return root.create_button(icons::right).set_callback(right).set_userdata(*this);
}


node&
menu_handler::
create_down_button(operating_node&  root) noexcept
{
  return root.create_button(icons::down).set_callback(down).set_userdata(*this);
}




}}




