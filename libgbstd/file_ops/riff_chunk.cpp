#include"libgbstd/file_op.hpp"



namespace gbstd{




struct
riff_chunk::
private_data
{
  size_t  reference_count;

  uint32_t  size;

  uint8_t  data[0];

};


riff_chunk&
riff_chunk::
operator=(const riff_chunk&   rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++m_data->reference_count;
        }
    }


  return *this;
}


riff_chunk&
riff_chunk::
operator=(riff_chunk&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




void
riff_chunk::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
          free(m_data)         ;
               m_data = nullptr;
        }


      m_data = nullptr;
    }
}


void
riff_chunk::
load_file(const char*  filepath) noexcept
{
  unrefer();

  auto  f = fopen(filepath,"rb");

    if(f)
    {
      riff_id  id;

      uint32_t  size;

      id[0] = fgetc(f);
      id[1] = fgetc(f);
      id[2] = fgetc(f);
      id[3] = fgetc(f);

      size  = (fgetc(f)    );
      size |= (fgetc(f)<< 8);
      size |= (fgetc(f)<<16);
      size |= (fgetc(f)<<24);

        if(ferror(f))
        {
          printf("%s ファイルエラー\n",filepath);
        }

      else
        if(feof(f) || (id != riff_id('R','I','F','F')))
        {
          printf("%sは有効なRIFFではないn",filepath);
        }

      else
        {
          m_data = static_cast<private_data*>(malloc(sizeof(private_data)+size));

          m_data->reference_count = 1;
          m_data->size = size;

            if(fread(m_data->data,1,size,f) != size)
            {
              printf("%s %zdバイト読み込めない\n",filepath,size);

              free(m_data)         ;
                   m_data = nullptr;
            }
        }


      fclose(f);
    }

  else
    {
      printf("%sが開けない１\n",filepath);
    }
}




uint32_t
riff_chunk::
size() const noexcept
{
  return m_data->size;
}


const uint8_t*
riff_chunk::
data() const noexcept
{
  return m_data->data;
}


}




