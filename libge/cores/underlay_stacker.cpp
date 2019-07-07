#include"libge/core.hpp"




namespace ge{




class
underlay_stacker: public gbstd::widgets::node
{
  gbstd::widgets::label&    m_counter_label;
  gbstd::widgets::label&  m_switching_label;

  core&  m_core;

  static void   psh(gbstd::widgets::button_event  evt) noexcept;
  static void   pop(gbstd::widgets::button_event  evt) noexcept;
  static void   swi(gbstd::widgets::button_event  evt) noexcept;

public:
  underlay_stacker(gbstd::widgets::operating_node&  root, core&  cor) noexcept;

  void  update_counter_label() noexcept;

};




void
underlay_stacker::
psh(gbstd::widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  stk = evt->get_userdata<underlay_stacker>();

      stk.m_core.add_underlay();

      stk.update_counter_label();

      stk.m_core.request_redraw();
    }
}


void
underlay_stacker::
pop(gbstd::widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  stk = evt->get_userdata<underlay_stacker>();

      auto&  dsp = stk.m_core.get_display();

        if(dsp.get_number_of_underlays())
        {
          dsp.pop_underlay();

          stk.update_counter_label();

          stk.m_core.request_redraw();
        }
    }
}


void
underlay_stacker::
swi(gbstd::widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  stk = evt->get_userdata<underlay_stacker>();

      auto&  dsp = stk.m_core.get_display();

        if(stk.m_core.test_underlay())
        {
          stk.m_core.hide_underlay();

          stk.m_switching_label.set_string_without_reform(u"show");
        }

      else
        {
          stk.m_core.show_underlay();

          stk.m_switching_label.set_string_without_reform(u"hide");
        }


      stk.m_core.request_redraw();
    }
}




underlay_stacker::
underlay_stacker(gbstd::widgets::operating_node&  root, core&  cor) noexcept:
m_switching_label(root.create_label(u"hide").set_color(gbstd::colors::black)),
m_counter_label(  root.create_label(u" 0").set_color(gbstd::colors::white)),
m_core(cor)
{
  auto&  psh_btn = root.create_button(u"push",gbstd::colors::black).set_callback(psh);
  auto&  pop_btn = root.create_button(u"pop",gbstd::colors::black).set_callback(pop);
  auto&  swi_btn = root.create_button(m_switching_label).set_callback(swi);

  gbstd::widgets::set_userdata({psh_btn,pop_btn,swi_btn},*this);


  add_child_as_column({m_counter_label,psh_btn,pop_btn,swi_btn});
}




void
underlay_stacker::
update_counter_label() noexcept
{
  gbstd::string_form  sf;

  m_counter_label.set_string(sf("%2d",m_core.get_display().get_number_of_underlays()));
}




gbstd::widgets::node&
core::
create_underlay_widget(gbstd::widgets::operating_node&  root) noexcept
{
  auto&  stk = *new underlay_stacker(root,*this);

  return root.create_frame(u"underlay").set_content(stk);
}




}




