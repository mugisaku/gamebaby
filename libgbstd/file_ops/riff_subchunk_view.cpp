#include"libgbstd/file_op.hpp"



namespace gbstd{




riff_subchunk_view&
riff_subchunk_view::
operator=(const riff_chunk&  src) noexcept
{
  assign(src);

  return *this;
}




void
riff_subchunk_view::
assign(const riff_chunk&  src) noexcept
{
  auto     p = src.data();
  auto  size = src.size();

    if(size < 4)
    {
      printf("不正なチャンク\n");
    }


  m_id[0] = *p++;
  m_id[1] = *p++;
  m_id[2] = *p++;
  m_id[3] = *p++;

  m_size = (size-4);

  m_data = p;
}


void
riff_subchunk_view::
assign(const uint8_t*  p) noexcept
{
  m_id[0] = *p++;
  m_id[1] = *p++;
  m_id[2] = *p++;
  m_id[3] = *p++;

  m_size = bget_le32(p);

  p += 4;

  m_data = p;
}


void
riff_subchunk_view::
print() const noexcept
{
  printf("id: ");

  m_id.print();

  printf("\nsize: %d\n",m_size);
}


}




