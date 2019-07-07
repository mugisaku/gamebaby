#include"libgbstd/file_op.hpp"



namespace gbstd{




void
wave_format::
update()
{
  const int  number_of_bytes_per_sample = (m_number_of_bits_per_sample ==  8)? 1
                                         :(m_number_of_bits_per_sample == 16)? 2
                                         :                                     0;

    if(((m_number_of_channels != 1) && (m_number_of_channels != 2))  ||
       !number_of_bytes_per_sample  ||
       !m_number_of_bits_per_sample ||
       !m_sampling_rate)
    {
      throw 0;
    }


  m_id = 1;
  m_block_size = number_of_bytes_per_sample*m_number_of_channels;
  m_data_rate  = m_sampling_rate*m_block_size;
}


void
wave_format::
read_from(const riff_subchunk_view&  rv) noexcept
{
  auto  p = rv.data();

  m_id = bget_le16(p);

    if(m_id != 1)
    {
      printf("リニアPCMではない\n");
      return;
    }


  p += 2;

  m_number_of_channels = bget_le16(p);

  p += 2;

  m_sampling_rate = bget_le32(p);

  p += 4;

  m_data_rate = bget_le32(p);

  p += 4;

  m_block_size = bget_le16(p);

  p += 2;

  m_number_of_bits_per_sample = bget_le16(p);

  p += 2;
}


void
wave_format::
save_to_file(FILE*  f) const noexcept
{
  fput_le16(m_id,f);
  fput_le16(m_number_of_channels,f);
  fput_le32(m_sampling_rate,f);
  fput_le32(m_data_rate,f);
  fput_le16(m_block_size,f);
  fput_le16(m_number_of_bits_per_sample,f);
}


void
wave_format::
print() const noexcept
{
  printf("       number of channels: %8d\n",m_number_of_channels);
  printf("            sampling rate: %8d\n",m_sampling_rate);
  printf("                data rate: %8d\n",m_data_rate);
  printf("               block size: %8d\n",m_block_size);
  printf("number of bits per sample: %8d\n",m_number_of_bits_per_sample);
}


}




