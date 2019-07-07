#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




frame_control&
frame_control::
assign(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

  m_sequence_number   = bv.get_be32();
  m_width             = bv.get_be32();
  m_height            = bv.get_be32();
  m_x_offset          = bv.get_be32();
  m_y_offset          = bv.get_be32();
  m_delay_numerator   = bv.get_be16();
  m_delay_denominator = bv.get_be16();

  m_dispose_type = static_cast<dispose_type>(bv.get_8());
  m_blend_type   = static_cast<  blend_type>(bv.get_8());

  return *this;
}


chunk
frame_control::
make_chunk() const noexcept
{
  binary  bin(26);

  binary_cursor  bc(bin);

  bc.put_be32(m_sequence_number);
  bc.put_be32(m_width);
  bc.put_be32(m_height);
  bc.put_be32(m_x_offset);
  bc.put_be32(m_y_offset);
  bc.put_be16(m_delay_numerator);
  bc.put_be16(m_delay_denominator);

  bc.put_8(static_cast<int>(m_dispose_type));
  bc.put_8(static_cast<int>(m_blend_type)  );

  return chunk(std::move(bin),chunk_name("fcTL"));
}




void
frame_control::
print() const noexcept
{
}


}




