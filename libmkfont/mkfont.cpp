#include"mkfont.hpp"




std::vector<character>
g_characters;


std::vector<combined>
g_combineds =
{
#include"combineds.txt"
};


character  g_null;


std::reference_wrapper<character>  g_current_character = std::ref(g_null);
std::reference_wrapper<combined>   g_current_combined  = std::ref(g_combineds[0]);


namespace{
std::string  g_path;

character*  g_table[0x10000];
}


void
initialize(std::string_view  path) noexcept
{
  auto  f = fopen(path.data(),"rb");

    if(f)
    {
      g_path = path;

        for(;;)
        {
          int  iarr[9];

            if(fscanf(f,"{%x,{%x,%x,%x,%x,%x,%x,%x,%x,}},\n",iarr+0,
              iarr+1,
              iarr+2,
              iarr+3,
              iarr+4,
              iarr+5,
              iarr+6,
              iarr+7,
              iarr+8) != 9)
            {
              break;
            }


          character  c = {
            static_cast<uint16_t>(iarr[0]),
            static_cast<uint8_t>( iarr[1]),
            static_cast<uint8_t>( iarr[2]),
            static_cast<uint8_t>( iarr[3]),
            static_cast<uint8_t>( iarr[4]),
            static_cast<uint8_t>( iarr[5]),
            static_cast<uint8_t>( iarr[6]),
            static_cast<uint8_t>( iarr[7]),
            static_cast<uint8_t>( iarr[8]),
          };


          g_characters.emplace_back(c);
        }


      fclose(f);
    }


    for(auto&  c: g_characters)
    {
      g_table[c.m_unicode] = &c;
    }


  g_current_character = std::ref(g_characters[0]);
}


character&
get_character_by_unicode(char16_t  unicode) noexcept
{
  static character  null;

  auto  ptr = g_table[unicode];

    if(!ptr)
    {
      return null;
    }


  return *ptr;
}


void
character::
render(const canvas&  cv, int  pixel_size) const noexcept
{
  auto  src = m_data;

    for(int  y = 0;  y < m_size;  ++y)
    {
      auto  code = *src++;

        for(int  x = 0;  x < m_size;  ++x)
        {
          auto&  white = colors::white;
          auto&  black = colors::black;

          cv.fill_rectangle((code&0x80)? white:black,pixel_size*x,
                                                     pixel_size*y,
                                                     pixel_size,
                                                     pixel_size);

          code <<= 1;
        }
    }
}




namespace{
void
up_chr(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_chrsel.up();
    }
}


void
down_chr(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_chrsel.down();
    }
}


void
up_cmb(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_cmbsel.up();
    }
}


void
down_cmb(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      evt->get_userdata<editor>().m_cmbsel.down();
    }
}


void
slide_chrtbl(widgets::slider_event  evt) noexcept
{
  evt->get_userdata<editor>().m_chrsel.set_offset(evt.get_value());
}


void
shu(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  c = g_current_character.get();

        for(int  i = 0;  i < character::m_size-1;  ++i)
        {
          c.m_data[i] = c.m_data[i+1];
        }


      c.m_data[character::m_size-1] = 0;

      evt->get_userdata<editor>().update();
    }
}


void
shl(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  c = g_current_character.get();

        for(int  i = 0;  i < character::m_size;  ++i)
        {
          c.m_data[i] <<= 1;
        }


      evt->get_userdata<editor>().update();
    }
}


void
shr(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  c = g_current_character.get();

        for(int  i = 0;  i < character::m_size;  ++i)
        {
          c.m_data[i] >>= 1;
        }


      evt->get_userdata<editor>().update();
    }
}


void
shd(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  c = g_current_character.get();

        for(int  i = 1;  i < character::m_size;  ++i)
        {
          c.m_data[character::m_size-i] = c.m_data[character::m_size-1-i];
        }


      c.m_data[0] = 0;

      evt->get_userdata<editor>().update();
    }
}


void
save(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto  f = fopen(g_path.data(),"wb");

        if(f)
        {
            for(auto  cptr: g_table)
            {
                if(cptr)
                {
                  auto&  c = *cptr;

                  fprintf(f,"{0x%04X,{",c.m_unicode);

                    for(int  i = 0;  i < character::m_size;  ++i)
                    {
                      fprintf(f,"0x%02X,",c.m_data[i]);
                    }


                  fprintf(f,"}},\n");
                }
            }


          fclose(f);
        }
    }
}


void
save_as_combined(widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto  f = fopen("font_data.txt","wb");

        if(f)
        {
            for(auto&  c: g_combineds)
            {
              auto&  u = get_character_by_unicode(c.m_upper);
              auto&  l = get_character_by_unicode(c.m_lower);

              fprintf(f,"{0x%04X,{",c.m_unicode);

                for(int  y = 0;  y < character::m_size;  ++y)
                {
                  fprintf(f,"0x%02X,",u.m_data[y]);
                }


                for(int  y = 0;  y < character::m_size;  ++y)
                {
                  fprintf(f,"0x%02X,",l.m_data[y]);
                }


              fprintf(f,"}},\n");
            }


          fclose(f);
        }
    }
}
}


editor::
editor(widgets::operating_node&  root) noexcept:
m_bitmap(*this),
m_chrsel(*this),
m_cmbsel(*this),
m_character_label(root.create_label().set_string(u"U+0000")),
m_selected_label(root.create_label().set_string(u"U+0000")),
m_character_table_slider(root.create_vertical_slider().set_length(80).set_value_max(60)),
m_character_up_button(root.create_button().set_content(root.create_label().set_string(u"↑"))),
m_character_down_button(root.create_button().set_content(root.create_label().set_string(u"↓"))),
m_combined_up_button(root.create_button().set_content(root.create_label().set_string(u"↑"))),
m_combined_down_button(root.create_button().set_content(root.create_label().set_string(u"↓"))),
m_shift_up_button(root.create_button().set_content(root.create_label().set_string(u"Shift ↑"))),
m_shift_left_button(root.create_button().set_content(root.create_label().set_string(u"Shift ←"))),
m_shift_right_button(root.create_button().set_content(root.create_label().set_string(u"Shift →"))),
m_shift_down_button(root.create_button().set_content(root.create_label().set_string(u"Shift ↓"))),
m_save_button(root.create_button().set_content(root.create_label().set_string(u"Save"))),
m_save_as_combined_button(root.create_button().set_content(root.create_label().set_string(u"Save as Combined")))
{
  m_character_up_button.set_callback(    up_chr).set_userdata(*this);
  m_character_down_button.set_callback(down_chr).set_userdata(*this);
  m_combined_up_button.set_callback(    up_cmb).set_userdata(*this);
  m_combined_down_button.set_callback(down_cmb).set_userdata(*this);
  m_save_button.set_callback(            save).set_userdata(*this);
  m_save_as_combined_button.set_callback(save_as_combined).set_userdata(*this);

  m_shift_up_button.set_callback(   shu).set_userdata(*this);
  m_shift_left_button.set_callback( shl).set_userdata(*this);
  m_shift_right_button.set_callback(shr).set_userdata(*this);
  m_shift_down_button.set_callback( shd).set_userdata(*this);

  m_character_table_slider.set_callback(slide_chrtbl).set_userdata(*this);
}



void
editor::
update() noexcept
{
  m_bitmap.request_redraw();
  m_chrsel.request_redraw();
  m_cmbsel.request_redraw();
}




