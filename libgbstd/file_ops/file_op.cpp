#include"libgbstd/file_op.hpp"




namespace gbstd{


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




