#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




int
dial_event::
get_new_value() const noexcept
{
  return (*this)->get_current();
}




dial::
dial(operating_node&  root) noexcept:
m_down_show(  root.create_iconshow()),
m_up_show(    root.create_iconshow()),
m_down_button(root.create_button()),
m_up_button(  root.create_button()),
m_label(      root.create_label())
{
  m_down_show.get().set_icon_list({&icons::left ,&icons::sunken_left });
  m_up_show.get().set_icon_list(  {&icons::right,&icons::sunken_right});
  m_down_button.get().set_content(m_down_show.get()).set_callback(down);
  m_up_button.get().set_content(    m_up_show.get()).set_callback(  up);

  m_down_button.get().set_userdata(*this);
  m_up_button.get().set_userdata(  *this);

  add_child_as_row({m_down_button.get(),m_up_button.get(),m_label.get()});
}




dial&
dial::
set_max(int  v) noexcept
{
  m_max = v;

  update_label();

  return *this;
}


dial&
dial::
set_min(int  v) noexcept
{
  m_min = v;

  update_label();

  return *this;
}


dial&
dial::
set_current(int  v) noexcept
{
  m_current = std::min(m_max,std::max(m_min,v));

  update_label();

  return *this;
}


void
dial::
update_label() noexcept
{
  string_form  fs;

  constexpr const char*  format = "%2d/%2d";

  m_label.get().set_string(fs(format,m_current,m_max));
}


void
dial::
up(button_event  evt) noexcept
{
  auto&  d = evt->get_userdata<dial>();

  d.m_up_show.get().set_index(evt.is_press()? 1:0);

    if(evt.is_release())
    {
        if(d.m_current < d.m_max)
        {
          auto  old_value = d.m_current++;

            if(d.m_callback)
            {
              d.m_callback({d,old_value});
            }


          d.update_label();
        }
    }
}


void
dial::
down(button_event  evt) noexcept
{
  auto&  d = evt->get_userdata<dial>();

  d.m_down_show.get().set_index(evt.is_press()? 1:0);

    if(evt.is_release())
    {
        if(d.m_current > d.m_min)
        {
          auto  old_value = d.m_current--;

            if(d.m_callback)
            {
              d.m_callback({d,old_value});
            }


          d.update_label();
        }
    }
}


}}




