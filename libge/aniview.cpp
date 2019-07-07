#include"libge/ge.hpp"
#include"libgbpng/png.hpp"




namespace ge{




namespace{
const uint32_t  g_time_table[] = {1000,600,200,80};
}


void
aniview::
psh(gbstd::widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  aniv = evt->get_userdata<aniview>();

      aniv.m_points.emplace_back(aniv.m_core.get_pixel_base_point());

      aniv.update_state_label();
    }
}


void
aniview::
pop(gbstd::widgets::button_event  evt) noexcept
{
    if(evt.is_release())
    {
      auto&  aniv = evt->get_userdata<aniview>();

        if(aniv.m_points.size())
        {
          aniv.m_points.pop_back();

          aniv.update_state_label();
        }
    }
}


void
aniview::
dial_callback(gbstd::widgets::dial_event  evt) noexcept
{
  auto&  aniv = evt->get_userdata<aniview>();

  aniv.m_delay_time = g_time_table[evt.get_new_value()-1];
}




aniview::
aniview(gbstd::widgets::operating_node&  root, core&  cor) noexcept:
m_core(cor),
m_state_label(root.create_label(u"--/ 0").set_color(gbstd::colors::white))
{
  auto&  d = root.create_dial();

  d.set_min(1)
   .set_max(4)
   .set_callback(dial_callback)
  ;


  auto&  psh_btn = root.create_button(u"Push",gbstd::colors::black).set_callback(psh);
  auto&  pop_btn = root.create_button(u"Pop",gbstd::colors::black).set_callback(pop);


  gbstd::widgets::set_userdata({d,psh_btn,pop_btn},*this);

  auto&  op_col = root.create_table_column({psh_btn,pop_btn});

  m_delay_time = g_time_table[0];
  m_next_time  =               0;


  auto&  speed_frame = root.create_frame("speed").set_content(d);

  auto&  row = root.create_table_row({m_state_label,op_col,speed_frame});

  add_child(row,{cor.get_source().get_cell_width(),0});

  clear();
}




void
aniview::
clear() noexcept
{
  m_index = 0;

  update_state_label();

  request_redraw();
}


void
aniview::
check() noexcept
{
    if(m_points.size())
    {
        if(gbstd::g_time >= m_next_time)
        {
          m_next_time = (gbstd::g_time+m_delay_time);

            if(++m_index >= m_points.size())
            {
              m_index = 0;
            }


          update_state_label();

          request_redraw();
        }
    }
}


namespace{
void
transfer(const gbstd::canvas  src_cv, gbpng::direct_color_image&  dst_img) noexcept
{
  int  w = src_cv.get_width() ;
  int  h = src_cv.get_height();

  auto  dst = dst_img.get_row_pointer(0);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      auto  color = src_cv.get_pixel_pointer(x,y)->color;

        if(color)
        {
          *dst++ = color.get_r255();
          *dst++ = color.get_g255();
          *dst++ = color.get_b255();
          *dst++ = 0xFF;
        }

      else
        {
          *dst++ = 0;
          *dst++ = 0;
          *dst++ = 0;
          *dst++ = 0;
        }
    }}
}
}


void
aniview::
save_as_apng(const char*  filepath) const noexcept
{
    if(m_points.size())
    {
      std::vector<uint8_t>  buf;

        try
        {
          using namespace gbpng;

          auto  it     = m_points.begin();
          auto  it_end = m_points.end();

          auto&  src = m_core.get_source();
          auto&  dsp = m_core.get_display();

          const int  w = src.get_cell_width() ;
          const int  h = src.get_cell_height();

          image_header  ihdr(w,h,pixel_format::rgba);

          animation_builder  ani(ihdr,m_delay_time);

          direct_color_image  dst_img(w,h);

          gbstd::image  tmp_img(w,h);

          gbstd::canvas  tmp_cv(tmp_img);

          bool  show_underlay = m_core.test_underlay();

            while(it != it_end)
            {
              dsp.render_background(tmp_cv,2);

                if(show_underlay)
                {
                  dsp.render_underlay(tmp_cv,1);
                }


              tmp_cv.draw_canvas(src.get_canvas(*it++),0,0);

              transfer(tmp_cv,dst_img);

              ani.append(dst_img);
            }


          auto  ls = ani.build(0);

          buf.resize(ls.calculate_stream_size());

          ls.write_png_to_memory(buf.data());
        }


        catch(std::exception&  e)
        {
          printf("error: %s\n",e.what());
        }



#ifdef __EMSCRIPTEN__
      gbstd::update_common_blob(buf.data(),buf.size());
      gbstd::download_common_blob(filepath);
#else
      gbstd::write_to_file(buf.data(),buf.size(),filepath);
#endif
    }
}


void
aniview::
process_before_reform() noexcept
{
  int  w = m_core.get_source().get_cell_width() ;
  int  h = m_core.get_source().get_cell_height();

  set_minimal_content_width( w);
  set_minimal_content_height(h);
}


void
aniview::
update_state_label() noexcept
{
    if(m_points.empty())
    {
      m_state_label.set_string_without_reform("--/ 0");
    }

  else
    {
      gbstd::string_form  sf;

      m_state_label.set_string_without_reform(sf("%2d/%2d",m_index+1,m_points.size()));
    }
}


void
aniview::
render(const gbstd::canvas&  cv) noexcept
{
    if(m_index < m_points.size())
    {
      auto  pt = m_points[m_index];

      auto&  dsp = m_core.get_display();
      auto&  src = m_core.get_source();

      dsp.render_background({cv,0,0,src.get_cell_width(),src.get_cell_height()},2);

        if(m_core.test_underlay())
        {
          dsp.render_underlay(cv,1);
        }


      cv.draw_canvas(m_core.get_source().get_canvas(pt),0,0);
    }
}


gbstd::widgets::node&
aniview::
create_save_button(gbstd::widgets::operating_node&  root) noexcept
{
  auto&  btn = root.create_button(u"save as APNG",gbstd::colors::black);

  btn
    .set_callback([](gbstd::widgets::button_event  evt){
      auto&  aniv = evt->get_userdata<aniview>();

        if(evt.is_release())
        {
          aniv.save_as_apng("noName.apng");
        }
    })
  ;


  btn.set_userdata(*this);

  return btn;
}




}




