#include"libsns/sns_sha256.hpp"




namespace sns{




void
sha256_hash::
reset() noexcept
{
  m_data[0] = 0x6a09e667;
  m_data[1] = 0xbb67ae85;
  m_data[2] = 0x3c6ef372;
  m_data[3] = 0xa54ff53a;
  m_data[4] = 0x510e527f;
  m_data[5] = 0x9b05688c;
  m_data[6] = 0x1f83d9ab;
  m_data[7] = 0x5be0cd19;
}


void
sha256_hash::
print() const noexcept
{
    for(auto  v: m_data)
    {
      printf("%08x",v);
    }
}




namespace{
constexpr uint32_t
rotr(uint32_t  v, int  amount) noexcept
{
  return (v<<(32-amount))|(v>>amount);
}
}




void
sha256_hash_generator::
extend_message(uint32_t*  w) const noexcept
{
    {
      const uint8_t*  src = m_block_buffer;

        for(int  i = 0;  i < 16;  ++i)
        {
          w[i]  = (*src++)<<24;
          w[i] |= (*src++)<<16;
          w[i] |= (*src++)<< 8;
          w[i] |= (*src++)    ;
        }
    }


    for(int  i = 16;  i < 64;  ++i)
    {
      constexpr auto  s0 = [](uint32_t  x) noexcept->uint32_t{return rotr(x, 7)^rotr(x,18)^(x>> 3);};
      constexpr auto  s1 = [](uint32_t  x) noexcept->uint32_t{return rotr(x,17)^rotr(x,19)^(x>>10);};

      w[i] = w[i-16]+s0(w[i-15])+w[i-7]+s1(w[i-2]);
    }
}




void
sha256_hash_generator::
compute(const uint32_t*  w) noexcept
{
  constexpr const uint32_t  k[] =
  {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };


  auto  a = m_data[0];
  auto  b = m_data[1];
  auto  c = m_data[2];
  auto  d = m_data[3];
  auto  e = m_data[4];
  auto  f = m_data[5];
  auto  g = m_data[6];
  auto  h = m_data[7];

    for(int  i = 0;  i < 64;  ++i)
    {
      constexpr auto  maj = [](uint32_t  a, uint32_t  b, uint32_t  c) noexcept->uint32_t{return (a&b)^(a&c)^(b&c);};
      constexpr auto   ch = [](uint32_t  e, uint32_t  f, uint32_t  g) noexcept->uint32_t{return (e&f)^((~e)&g);};
      constexpr auto   s0 = [](uint32_t  a) noexcept->uint32_t{return rotr(a,2)^rotr(a,13)^rotr(a,22);};
      constexpr auto   s1 = [](uint32_t  e) noexcept->uint32_t{return rotr(e,6)^rotr(e,11)^rotr(e,25);};

      uint32_t  temp1 = h+s1(e)+ch(e,f,g)+k[i]+w[i];
      uint32_t  temp2 = s0(a)+maj(a,b,c);

      h = g;
      g = f;
      f = e;
      e = d+temp1;
      d = c;
      c = b;
      b = a;
      a = temp1+temp2;

//printf("[%2d] %08x %08x %08x %08x %08x %08x %08x %08x\n",i,a,b,c,d,e,f,g,h);
    }


  m_data[0] += a;
  m_data[1] += b;
  m_data[2] += c;
  m_data[3] += d;
  m_data[4] += e;
  m_data[5] += f;
  m_data[6] += g;
  m_data[7] += h;
}


sha256_hash_generator&
sha256_hash_generator::
reset() noexcept
{
  sha256_hash::reset();

  m_input_size = 0;

  clear_block_buffer();

  return *this;
}


sha256_hash_generator&
sha256_hash_generator::
finish() noexcept
{
  uint64_t  num_bits = 8*m_input_size    ;
                         m_input_size = 0;

  push(static_cast<uint8_t>(0x80));

    if(m_block_size > (sizeof(m_block_buffer)-8))
    {
      process();
    }


  uint8_t*  p = &m_block_buffer[sizeof(m_block_buffer)-1];

  *p-- = (num_bits    );
  *p-- = (num_bits>> 8);
  *p-- = (num_bits>>16);
  *p-- = (num_bits>>24);
  *p-- = (num_bits>>32);
  *p-- = (num_bits>>40);
  *p-- = (num_bits>>48);
  *p-- = (num_bits>>56);

  process();

  return *this;
}


void
sha256_hash_generator::
clear_block_buffer() noexcept
{
  std::memset(m_block_buffer,0,sizeof(m_block_buffer));

  m_block_size = 0;
}


void
sha256_hash_generator::
process() noexcept
{
  uint32_t  w[64] = {0};

  extend_message(w);
         compute(w);

  clear_block_buffer();
}


void
sha256_hash_generator::
push(int  c) noexcept
{
  m_block_buffer[m_block_size++] = c;

  ++m_input_size;

    if(m_block_size >= sizeof(m_block_buffer))
    {
      process();
    }
}


void
sha256_hash_generator::
push_le16(uint16_t  c) noexcept
{
  push((c   )&0xFF);
  push((c>>8)&0xFF);
}


void
sha256_hash_generator::
push_be16(uint16_t  c) noexcept
{
  push((c>>8)&0xFF);
  push((c   )&0xFF);
}


void
sha256_hash_generator::
push_le32(uint32_t  c) noexcept
{
  push((c    )&0xFF);
  push((c>> 8)&0xFF);
  push((c>>16)&0xFF);
  push((c>>24)&0xFF);
}


void
sha256_hash_generator::
push_be32(uint32_t  c) noexcept
{
  push((c>>24)&0xFF);
  push((c>>16)&0xFF);
  push((c>> 8)&0xFF);
  push((c    )&0xFF);
}


void
sha256_hash_generator::
push_le64(uint64_t  c) noexcept
{
  push((c    )&0xFF);
  push((c>> 8)&0xFF);
  push((c>>16)&0xFF);
  push((c>>24)&0xFF);
  push((c>>32)&0xFF);
  push((c>>40)&0xFF);
  push((c>>48)&0xFF);
  push((c>>56)&0xFF);
}


void
sha256_hash_generator::
push_be64(uint64_t  c) noexcept
{
  push((c>>56)&0xFF);
  push((c>>48)&0xFF);
  push((c>>40)&0xFF);
  push((c>>32)&0xFF);
  push((c>>24)&0xFF);
  push((c>>16)&0xFF);
  push((c>> 8)&0xFF);
  push((c    )&0xFF);
}




}




