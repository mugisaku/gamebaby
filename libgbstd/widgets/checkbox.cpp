#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




struct
checkbox::
shared_data
{
  uint32_t  m_reference_count=0;

  void*  m_userdata=nullptr;

  void  (*m_callback)(checkbox_event  evt)=nullptr;

  checkbox*  m_first=nullptr;
  checkbox*  m_last =nullptr;

  const checkbox*  m_last_changed=nullptr;

  bool  m_radio_mark=false;

};




checkbox::
checkbox(operating_node&  root) noexcept:
m_label(root.create_label()),
m_iconshow(root.create_iconshow()),
m_data(new shared_data),
m_entry_number(0)
{
  m_iconshow.get().set_icon_list({&icons::unchecked,&icons::checked});

  be_autonomous();

  m_data->m_first = this;
  m_data->m_last  = this;

  add_child_as_row({m_iconshow.get(),m_label.get()});

  ++m_data->m_reference_count;
}


checkbox::
checkbox(operating_node&  root, const checkbox&  member) noexcept:
m_label(root.create_label()),
m_iconshow(root.create_iconshow()),
m_data(member.m_data),
m_entry_number(member.m_data->m_last->m_entry_number+1)
{
  m_iconshow.get().set_icon_list({&icons::unchecked,&icons::checked});

  be_autonomous();

  m_data->m_last->m_next = this;
  m_data->m_last         = this;

  add_child_as_row({m_iconshow.get(),m_label.get()});

  ++m_data->m_reference_count;
}


checkbox::
~checkbox()
{
    if(m_data)
    {
      auto&  refcount = m_data->m_reference_count;

        if(!--refcount)
        {
          delete m_data;
        }
    }
}





checkbox&
checkbox::
set_string(std::string_view  sv) noexcept
{
  m_label.get().set_string(sv);

  return *this;
}


checkbox&
checkbox::
set_string(std::u16string_view  sv) noexcept
{
  m_label.get().set_string(sv);

  return *this;
}


checkbox&
checkbox::
set_callback(void  (*cb)(checkbox_event)) noexcept
{
  m_data->m_callback = cb;

  return *this;
}




void*
checkbox::
get_common_userdata_internal() const noexcept
{
  return m_data->m_userdata;
}


const checkbox&
checkbox::
set_common_userdata_internal(void*  ptr) const noexcept
{
  m_data->m_userdata = ptr;

  return *this;
}




void
checkbox::
check() const noexcept
{
    if(is_checked())
    {
      return;
    }


  auto  cb = m_data->m_callback;

    if(m_data->m_radio_mark)
    {
        if(m_data->m_last_changed)
        {
          m_data->m_last_changed->m_iconshow.get().set_index(0);

            if(cb)
            {
              cb({*m_data->m_last_changed,checkbox_event::kind::unset});
            }
        }


      m_iconshow.get().set_index(1);

        if(cb)
        {
          cb({*this,checkbox_event::kind::set});
        }


      m_data->m_last_changed = this;
    }

  else
    {
      m_iconshow.get().set_index(1);

        if(cb)
        {
          cb({*this,checkbox_event::kind::set});
        }
    }
}


void
checkbox::
uncheck() const noexcept
{
  auto  cb = m_data->m_callback;

    if(!m_data->m_radio_mark && is_checked())
    {
      m_iconshow.get().set_index(0);

        if(cb)
        {
          cb({*this,checkbox_event::kind::unset});
        }
    }
}


void
checkbox::
on_mouse_act(point  mouse_pos) noexcept
{
    if(!g_input.test_mouse_left() && g_modified_input.test_mouse_left())
    {
        if(m_data->m_radio_mark)
        {
            if(!is_checked())
            {
              check();
            }
        }

      else
        {
            if(is_checked()){uncheck();}
          else              {  check();}
        }
    }
}




void
checkbox::
revise_to_radio() noexcept
{
  m_iconshow.get().set_icon_list({&icons::radio_unchecked,&icons::radio_checked});

  m_data->m_radio_mark = true;
}




}}




