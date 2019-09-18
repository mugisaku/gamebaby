#include"libgbstd/file_op.hpp"




namespace gbstd{




std::string
make_string_from_file(const char*  filepath) noexcept
{
  std::string  s;

  auto  f = fopen(filepath,"rb");

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s += c;
        }


      fclose(f);
    }

  else
    {
      printf("make_string_from_file error: %sは開けない\n",filepath);
    }


  return std::move(s);
}


void
write_to_file(const void*  ptr, size_t  size, const char*  filepath) noexcept
{
  auto  f = fopen(filepath,"wb");

    if(f)
    {
      auto  u8ptr = static_cast<const uint8_t*>(ptr);

        while(size--)
        {
          fputc(*u8ptr++,f);
        }


      fclose(f);
    }
}




}




