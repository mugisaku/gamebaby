#include"libge/core.hpp"




namespace ge{




core_source::
core_source(gbstd::widgets::operating_node&  root, core&  cor) noexcept:
m_core(cor),
m_size_label(root.create_label(u"   0x   0"))
{
}




core_source&
core_source::
set_cell_size(int  cell_w, int  cell_h) noexcept
{
  m_cell_width  = cell_w;
  m_cell_height = cell_h;

  return *this;
}


core_source&
core_source::
inflate(int  w, int  h) noexcept
{
  w *= m_cell_width;
  h *= m_cell_height;

  m_image.resize(w,h);

  update_size_label();

  return *this;
}


core_source&
core_source::
load(const std::vector<uint8_t>&  bin) noexcept
{
  gbstd::image  img;

  img.read_png_stream(bin.data());

    if(img.get_width())
    {
      int  cell_w = get_cell_width();
      int  cell_h = get_cell_height();

      int  max_w = std::max(img.get_width() ,m_image.get_width() );
      int  max_h = std::max(img.get_height(),m_image.get_height());

      int  min_w = std::min(img.get_width() ,m_image.get_width() );
      int  min_h = std::min(img.get_height(),m_image.get_height());

      m_image.resize(max_w,max_h);

      m_image.fill(gbstd::color());

        for(int  y = 0;  y < min_h;  ++y){
        for(int  x = 0;  x < min_w;  ++x){
          m_image.get_pixel_pointer(x,y)->color = img.get_pixel_pointer(x,y)->color;
        }}


      m_core.get_display().clear_underlay_stack();

      m_core.request_redraw();

      m_menu->get_view().get_table().resize(max_w/cell_w,max_h/cell_h);

      m_menu->update_y_offset_label();
      m_menu->request_redraw();

      update_size_label();
    }


  return *this;
}


core_source&
core_source::
update_size_label() noexcept
{
  gbstd::string_form  sf;

  m_size_label.set_string_without_reform(sf("%4dx%4d",m_image.get_width(),m_image.get_height()));

  return *this;
}


gbstd::canvas
core_source::
get_canvas(gbstd::point  pt) noexcept
{
  return {m_image,m_cell_width*pt.x,m_cell_height*pt.y,m_cell_width,m_cell_height};
}




gbstd::widgets::node&
core_source::
create_save_png_widget(gbstd::widgets::operating_node&  root) noexcept
{
  auto&  btn = root.create_button(u"save as PNG");

  btn
    .set_callback([](gbstd::widgets::button_event  evt){
      auto&  src = evt->get_userdata<core_source>();

        if(evt.is_release())
        {
          auto  bin = src.m_image.make_png_stream();

          constexpr const char*  filepath = "noName.png";

#ifdef __EMSCRIPTEN__
          gbstd::update_common_blob(bin.data(),bin.size());
          gbstd::download_common_blob(filepath);
#else
          gbstd::write_to_file(bin.data(),bin.size(),filepath);
#endif
        }
    })
  ;


  return btn.set_userdata(*this);
}


gbstd::widgets::node&
core_source::
create_save_txt_widget(gbstd::widgets::operating_node&  root) noexcept
{
  auto&  btn = root.create_button(u"save as C code");

  btn
    .set_callback([](gbstd::widgets::button_event  evt){
      auto&  src = evt->get_userdata<core_source>();

        if(evt.is_release())
        {
          auto  txt = src.m_image.make_txt_stream();

          constexpr const char*  filepath = "noName.txt";

#ifdef __EMSCRIPTEN__
          gbstd::update_common_blob(txt.data(),txt.size());
          gbstd::download_common_blob(filepath);
#else
          gbstd::write_to_file(txt.data(),txt.size(),filepath);
#endif
        }
    })
  ;


  return btn.set_userdata(*this);
}


gbstd::widgets::node&
core_source::
create_extend_widget(gbstd::widgets::operating_node&  root) noexcept
{
  auto&  ico = root.create_iconshow().set_icon_list({&gbstd::widgets::icons::plus});

  auto&  btn = root.create_button();

  btn
    .set_content(ico)
    .set_callback([](gbstd::widgets::button_event  evt){
      auto&  src = evt->get_userdata<core_source>();

        if(evt.is_release())
        {
          auto&  img = src.m_image;

          img.resize(img.get_width(),img.get_height()+src.m_cell_height);

          src.update_size_label();

          src.m_menu->get_view().get_table().resize(img.get_width() /src.m_cell_width,
                                                    img.get_height()/src.m_cell_height);

          src.m_menu->update_y_offset_label();
        }
    })
  ;


  return btn.set_userdata(*this);
}




}




