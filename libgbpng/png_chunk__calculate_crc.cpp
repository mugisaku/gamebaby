#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{


namespace{
uint32_t  g_table[256];


void
make_crc_table() noexcept
{
  uint32_t  c;

    for(int  n = 0;  n < 256;  n++)
    {
      c = n;

        for(int  k = 0;  k < 8;  k++)
        {
            if(c&1)
            {
              c = 0xedb88320^(c>>1);
            }

          else
            {
              c >>= 1;
            }
        }


     g_table[n] = c;
   }
}


}




uint32_t
chunk::
calculate_crc() const noexcept
{
  static bool  initialized;

    if(!initialized)
    {
      make_crc_table();

      initialized = true;
    }


  uint32_t  c = 0xFFFFFFFF;

  char  buf[5];

  m_name.print(buf);

    for(int  n = 0;  n < 4;  n++)
    {
      c = g_table[(c^buf[n])&0xFF]^(c>>8);
    }


    for(int  n = 0;  n < get_data_size();  n++)
    {
      c = g_table[(c^get_data()[n])&0xFF]^(c>>8);
    }


  return c^0xFFFFFFFF;
}


}




