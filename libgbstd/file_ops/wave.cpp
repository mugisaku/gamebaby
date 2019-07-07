#include"libgbstd/file_op.hpp"



namespace gbstd{




void
wave::
read_data(const riff_subchunk_view&  rv) noexcept
{
  m_length = rv.size();
  m_data   = rv.data();
}


wave&
wave::
assign(const riff_subchunk_view&  rv) noexcept
{
    if(rv.id() != riff_id('W','A','V','E'))
    {
      printf("無効なWAVE\n");
      return *this;
    }


  auto    p =   rv.data();
  auto  end = p+rv.size();

    while(p < end)
    {
      riff_subchunk_view  child_rv(p);

        if(child_rv.id() == riff_id('f','m','t',' '))
        {
          m_format.read_from(child_rv);
        }

      else
        if(child_rv.id() == riff_id('d','a','t','a'))
        {
          read_data(child_rv);
        }

      else
        if(child_rv.id() == riff_id('L','I','S','T'))
        {
        }

      else
        {
          child_rv.print();

          printf("不明なチャンク\n");

          break;
        }


      p = child_rv.data()+child_rv.size();
    }


  return *this;
}


wave&
wave::
assign(const void*  data, size_t  length, const wave_format&  fmt) noexcept
{
  m_format = fmt;

  m_length = length;
  m_data   = static_cast<const uint8_t*>(data);


  return *this;
}




void
wave::
save_to_file(FILE*  f) const noexcept
{
  fwrite("RIFF",1,4,f);

  fpos_t  riff_size_pos;

  fput_le32(4+4+4+2+2+4+4+2+2+4+4+length(),f);

  fwrite("WAVE",1,4,f);
  fwrite("fmt ",1,4,f);

  fput_le32(16,f);

  m_format.save_to_file(f);

  fwrite("data",1,4,f);

  fput_le32(length(),f);

    switch(m_format.get_number_of_bits_per_sample())
    {
  case( 8): fwrite(m_data,1,m_length,f);break;
  case(16): fwrite_le16(reinterpret_cast<const uint16_t*>(m_data),m_length/2,f);break;
  default:
      printf("invalid number of bits per sample\n");
      fwrite(m_data,1,length(),f);
    }
}


void
wave::
save_to_file(const char*  filepath) const noexcept
{
  auto  f = fopen(filepath,"wb");

    if(f)
    {
      save_to_file(f);

      fclose(f);
    }
}




std::vector<uint8_t>
wave::
to_binary() const noexcept
{
  std::vector<uint8_t>  bin;

  auto  f = tmpfile();

    if(f)
    {
      save_to_file(f);

      rewind(f);

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          bin.emplace_back(c);
        }


      fclose(f);
    }


  return std::move(bin);
}


void
wave::
print() const noexcept
{
  m_format.print();

  printf("data length: %8d\n",m_length);
}


}




