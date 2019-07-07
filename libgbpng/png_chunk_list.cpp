#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
constexpr uint8_t
g_signature[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};


const chunk
g_end_chunk(binary(),chunk_name("IEND"));
}




void
chunk_list::
push_front(chunk&&  chk) noexcept
{
  std::vector<chunk>  tmp;

  tmp.emplace_back(std::move(chk));

    for(auto&&  e: m_container)
    {
      tmp.emplace_back(std::move(e));
    }


  m_container = std::move(tmp);
}


void
chunk_list::
push_back(chunk&&  chk) noexcept
{
  m_container.emplace_back(std::move(chk));
}




uint32_t
chunk_list::
calculate_stream_size() const noexcept
{
  uint32_t  size = 8;

    for(auto&  chk: m_container)
    {
      size += chk.get_stream_size();
    }


  size += g_end_chunk.get_stream_size();

  return size;
}




void
chunk_list::
read_png_from_memory(const uint8_t*  p)
{
    if(std::memcmp(p,g_signature,8) != 0)
    {
      throw_error("signature is invalid\n");
    }


  p += 8;

  m_container.clear();

  chunk  tmp;

    for(;;)
    {
      tmp.read(p);

        if(tmp == "IEND")
        {
          break;
        }

      else
        {
          m_container.emplace_back(std::move(tmp));
        }
    }
}


void
chunk_list::
read_png_from_file(FILE*  f)
{
  std::vector<uint8_t>  buf;

    for(;;)
    {
      int  c = fgetc(f);

        if(feof(f))
        {
          break;
        }


        if(ferror(f))
        {
          throw_error("file error\n");
        }


      buf.emplace_back(c);
    }


  read_png_from_memory(buf.data());
}


void
chunk_list::
read_png_from_file(const char*  path)
{
  auto  f = file_wrapper(fopen(path,"rb"));

    if(!f)
    {
      throw_error("could not open file\n");
    }


  read_png_from_file(f);
}




void
chunk_list::
write_png_to_memory(uint8_t*  p) const noexcept
{
    for(auto  c: g_signature)
    {
      *p++ = c;
    }


    for(auto&  chk: m_container)
    {
      chk.write(p);
    }


  g_end_chunk.write(p);
}


void
chunk_list::
write_png_to_file(FILE*  f) const
{
  auto  size = calculate_stream_size();

  auto  ptr = std::make_unique<uint8_t[]>(size);

  write_png_to_memory(ptr.get());

    if(fwrite(ptr.get(),size,1,f) != 1)
    {
      throw_error("file writing error\n");
    }
}


void
chunk_list::
write_png_to_file(const char*  path) const
{
  auto  f = file_wrapper(fopen(path,"wb"));

    if(!f)
    {
      throw_error("could not open file\n");
    }


  write_png_to_file(f);
}




void
chunk_list::
print() const noexcept
{
    for(auto&  chk: m_container)
    {
      printf("{\n");
      chk.print();
      printf("}\n\n");
    }
}




}




