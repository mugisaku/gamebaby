#ifndef GBSTD_SHA256_HPP_WAS_INCLUDED
#define GBSTD_SHA256_HPP_WAS_INCLUDED


#include<cstdint>
#include<cstdio>
#include<cstring>
#include<cctype>
#include<string>
#include<string_view>




namespace gbstd{




class
sha256_hash
{
protected:
  uint32_t  m_data[8];

  void  reset() noexcept;

public:
  bool  operator==(const sha256_hash&  rhs) const noexcept{return std::memcmp(m_data,&rhs.m_data,sizeof(m_data)) == 0;}

  const uint32_t&  operator[](int  i) const noexcept{return m_data[i];}

  void  print() const noexcept;

  const uint32_t*  begin() const noexcept{return std::cbegin(m_data);}
  const uint32_t*    end() const noexcept{return std::cend(  m_data);}

};


class
sha256_hash_generator: public sha256_hash
{
  uint64_t  m_input_size=0;

  uint8_t  m_block_buffer[64]={0};

  int  m_block_size=0;

  void  clear_block_buffer() noexcept;

  void  process() noexcept;

  void  extend_message(uint32_t*  w) const noexcept;

  void  compute(const uint32_t*  w) noexcept;

public:
  sha256_hash_generator() noexcept{reset();}

  sha256_hash_generator&  reset() noexcept;

  sha256_hash_generator&  finish() noexcept;

  void  push(int  c) noexcept;

  void  push_le16(uint16_t  c) noexcept;
  void  push_be16(uint16_t  c) noexcept;
  void  push_le32(uint32_t  c) noexcept;
  void  push_be32(uint32_t  c) noexcept;
  void  push_le64(uint64_t  c) noexcept;
  void  push_be64(uint64_t  c) noexcept;

};


class
sha256_string
{
  char  m_data[(8*8)+1]={0};

public:
  sha256_string(                        ) noexcept{}
  sha256_string(const sha256_hash&  hash) noexcept{assign(hash);}

  sha256_string&  operator=(const sha256_hash&  hash) noexcept{return assign(hash);}
  sha256_string&     assign(const sha256_hash&  hash) noexcept
  {
    snprintf(m_data,sizeof(m_data),"%08x%08x%08x%08x%08x%08x%08x%08x",
      hash[0],hash[1],hash[2],hash[3],
      hash[4],hash[5],hash[6],hash[7]);

    return *this;
  }

  std::string_view  get_string() const noexcept{return std::string_view(m_data,8*8);}

};




}




#endif




