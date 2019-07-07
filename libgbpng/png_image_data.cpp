#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




image_data&
image_data::
assign(const std::vector<const chunk*>&  ls) noexcept
{
  size_t  size = 0;

    for(auto  chk: ls)
    {
      size += chk->get_data_size();
    }


  resize(size);

  auto  p = begin();

    for(auto  chk: ls)
    {
      auto  current_size = chk->get_data_size();

      std::memcpy(p,chk->begin(),current_size);

      p += current_size;
    }


  return *this;
}


image_data&
image_data::
assign(const uint8_t*  ptr, const image_header&  ihdr) noexcept
{
  auto  filtered = get_filtered(ptr,ihdr);

  binary::assign(filtered.get_compressed());

  return *this;
}




binary
image_data::
extract(const image_header&  ihdr) const noexcept
{
  auto  uncompressed = get_uncompressed();

  return get_unfiltered(uncompressed.get_data(),ihdr);
}




chunk
image_data::
make_chunk() const noexcept
{
  return chunk(*this,chunk_name("IDAT"));
}




void
image_data::
print() const noexcept
{
}


}




