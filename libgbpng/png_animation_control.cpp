#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




animation_control&
animation_control::
assign(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

  m_number_of_frames = bv.get_be32();
  m_number_of_plays  = bv.get_be32();

  return *this;
}




chunk
animation_control::
make_chunk() const noexcept
{
  binary  bin(8);

  binary_cursor  bc(bin);

  bc.put_be32(m_number_of_frames);
  bc.put_be32(m_number_of_plays );

  return chunk(std::move(bin),chunk_name("acTL"));
}




void
animation_control::
print() const noexcept
{
}


}




