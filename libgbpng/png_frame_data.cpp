#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




frame_data&
frame_data::
assign(uint32_t  seq_num, const std::vector<const chunk*>&  ls) noexcept
{
  m_sequence_number = seq_num++;

  size_t  size = 0;

    for(auto  chk: ls)
    {
      size += chk->get_data_size()-4;
    }


  resize(size);

  auto  p = begin();

    for(auto  chk: ls)
    {
      binary_view  bv(*chk);

      auto  n = bv.get_be32();

        if(n != seq_num)
        {
          printf("frame_data assign error: wrong sequence number %d %d\n",seq_num,n);
        }


      ++seq_num;

      auto  current_size = chk->get_data_size()-4;

      std::memcpy(p,bv.get_pointer(),current_size);

      p += current_size;
    }


  return *this;
}


frame_data&
frame_data::
assign(uint32_t  seq_num, image_data&&  idat) noexcept
{
  m_sequence_number = seq_num;

  binary::assign(std::move(idat));

  return *this;
}




chunk
frame_data::
make_chunk() const noexcept
{
  binary  bin(4+get_data_size());

  binary_cursor  bc(bin);

  bc.put_be32(m_sequence_number);

  std::memcpy(bc.get_pointer(),begin(),get_data_size());

  return chunk(std::move(bin),chunk_name("fdAT"));
}




void
frame_data::
print() const noexcept
{
}


}




