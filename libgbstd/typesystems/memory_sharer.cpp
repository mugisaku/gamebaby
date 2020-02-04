#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




void
memory_sharer::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--get_reference_count())
        {
          free(m_data);
        }


      m_data = nullptr;

      m_offset = 0;
      m_length = 0;
    }
}


uint64_t&
memory_sharer::
get_reference_count() const noexcept
{
  return *reinterpret_cast<uint64_t*>(m_data);
}


uint8_t*
memory_sharer::
get_memory_pointer() const noexcept
{
  return m_data+sizeof(uint64_t);
}


memory_sharer&
memory_sharer::
assign(const memory_sharer&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++get_reference_count();
        }


      m_offset = rhs.m_offset;
      m_length = rhs.m_length;
    }


  return *this;
}


memory_sharer&
memory_sharer::
assign(memory_sharer&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
      std::swap(m_offset,rhs.m_offset);
      std::swap(m_length,rhs.m_length);
    }


  return *this;
}


memory_sharer&
memory_sharer::
assign(uint32_t  length) noexcept
{
  unrefer();

  m_data = (uint8_t*)calloc(1,sizeof(uint64_t)+length);

  m_length = length;

  get_reference_count() = 1;

  return *this;
}


memory_sharer&
memory_sharer::
assign(const memory_sharer&  base, int  offset) noexcept
{
  assign(base);

  m_offset += offset;
  m_length -= offset;

  return *this;
}


memory_sharer&
memory_sharer::
assign(const memory_sharer&  base, int  offset, int  length) noexcept
{
  assign(base);

  m_offset += offset;
  m_length  = length;

  return *this;
}


memory_sharer
memory_sharer::
clone() const noexcept
{
  memory_sharer  m(m_length);

  std::memcpy(m.get_memory_pointer(),get_memory_pointer()+m_offset,m_length);

  return std::move(m);
}


memory_sharer&
memory_sharer::
copy(const memory_sharer&  src) noexcept
{
  auto  l = std::min(m_length,src.m_length);

  std::memcpy(get_memory_pointer(),src.get_memory_pointer(),l);

  return *this;
}




}}




