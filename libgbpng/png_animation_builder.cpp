#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




void
animation_builder::
reset(const image_header&  ihdr, uint32_t  delay_ms) noexcept
{
  m_palette.set_number_of_colors(0);

  m_buffer.clear();

  m_ihdr = ihdr;

  m_delay_ms = delay_ms;

  m_sequence_number = 0;

  m_number_of_frames = 0;
}




void
animation_builder::
append(const direct_color_image&  img)
{
    if((img.get_width()  != m_ihdr.get_width() ) ||
       (img.get_height() != m_ihdr.get_height()))
    {
      throw_error("size is mismatched");
    }


  frame_control  fctl;

  fctl.set_sequence_number(m_sequence_number++);
  fctl.set_width( m_ihdr.get_width() );
  fctl.set_height(m_ihdr.get_height());
  fctl.set_delay_numerator(m_delay_ms);
  fctl.set_delay_denominator(1000);

  m_buffer.emplace_back(fctl.make_chunk());

  auto  fmt       = m_ihdr.get_pixel_format();
  auto  bit_depth = m_ihdr.get_bit_depth();

  chunk  chk;  

    if(fmt == pixel_format::indexed)
    {
      auto  iimg = img.make_indexed_color_image(m_palette);

      auto  idat = iimg.get_image_data(bit_depth);

        if(m_buffer.size() == 1)
        {
          chk = idat.make_chunk();
        }

      else
        {
          frame_data  fdat(m_sequence_number++,std::move(idat));

          chk = fdat.make_chunk();
        }
    }

  else
    {
      auto  idat = img.get_image_data(fmt,bit_depth);

        if(m_buffer.size() == 1)
        {
          chk = idat.make_chunk();
        }

      else
        {
          frame_data  fdat(m_sequence_number++,std::move(idat));

          chk = fdat.make_chunk();
        }
    }


  m_buffer.emplace_back(std::move(chk));

  ++m_number_of_frames;
}




chunk_list
animation_builder::
build(uint32_t  number_of_plays) const
{
    if(!m_number_of_frames)
    {
      throw_error("have no frames");
    }


  chunk_list  ls;

  ls.push_back(m_ihdr.make_chunk());


  animation_control  actl;

  actl.set_number_of_frames(m_number_of_frames);
  actl.set_number_of_plays(number_of_plays);

  ls.push_back(actl.make_chunk());

    if(m_ihdr.get_pixel_format() == pixel_format::indexed)
    {
      ls.push_back(m_palette.make_chunk());
    }


    for(auto&  chk: m_buffer)
    {
      ls.push_back(chunk(chk));
    }


  return std::move(ls);
}




}




