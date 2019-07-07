#ifndef gbstd_file_op_HPP
#define gbstd_file_op_HPP


#include<cstdio>
#include<cstdint>
#include<cstdlib>
#include<utility>
#include<vector>


namespace gbstd{


void  write_to_file(const void*  ptr, size_t  size, const char*  filepath) noexcept;


inline
uint64_t
bget_le64(const uint8_t*  p) noexcept
{
  uint64_t  i;

  i  = (p[0]    );
  i |= (p[1]<< 8);
  i |= (p[2]<<16);
  i |= (p[3]<<24);
  i |= (static_cast<uint64_t>(p[4])<<32);
  i |= (static_cast<uint64_t>(p[5])<<40);
  i |= (static_cast<uint64_t>(p[6])<<48);
  i |= (static_cast<uint64_t>(p[7])<<56);

  return i;
}


inline
uint32_t
bget_le32(const uint8_t*  p) noexcept
{
  uint32_t  i;

  i  = (p[0]    );
  i |= (p[1]<< 8);
  i |= (p[2]<<16);
  i |= (p[3]<<24);

  return i;
}


inline
uint16_t
bget_le16(const uint8_t*  p) noexcept
{
  uint16_t  i;

  i  = (p[0]   );
  i |= (p[1]<<8);

  return i;
}


inline
void
fput_le32(uint32_t  i, FILE*  f) noexcept
{
  fputc((i    )&0xFF,f);
  fputc((i>> 8)&0xFF,f);
  fputc((i>>16)&0xFF,f);
  fputc((i>>24)&0xFF,f);
}


inline
void
fput_le64(uint64_t  i, FILE*  f) noexcept
{
  fputc((i    )&0xFF,f);
  fputc((i>> 8)&0xFF,f);
  fputc((i>>16)&0xFF,f);
  fputc((i>>24)&0xFF,f);
  fputc((i>>32)&0xFF,f);
  fputc((i>>40)&0xFF,f);
  fputc((i>>48)&0xFF,f);
  fputc((i>>56)&0xFF,f);
}


inline
void
fput_le16(uint16_t  i, FILE*  f) noexcept
{
  fputc((i    )&0xFF,f);
  fputc((i>> 8)&0xFF,f);
}


inline
void
fwrite_le16(const uint16_t*  ptr, size_t  n, FILE*  f) noexcept
{
    while(n--)
    {
      fput_le16(*ptr++,f);
    }
}


class riff_chunk;


class
riff_id
{
  char  m_data[4];

public:
  constexpr riff_id() noexcept: m_data{0}{}
  constexpr riff_id(char  c0, char  c1, char  c2, char  c3) noexcept: m_data{c0,c1,c2,c3}{}

  constexpr uint32_t  encode() const noexcept
  {
    return((m_data[0]<<24)|
           (m_data[1]<<16)|
           (m_data[2]<< 8)|
           (m_data[3]    ));
  }

  constexpr bool  operator==(const riff_id&  rhs) const noexcept
  {
    return(encode() == rhs.encode());
  }

  constexpr bool  operator!=(const riff_id&  rhs) const noexcept
  {
    return(encode() != rhs.encode());
  }

                  char&  operator[](int  i)       noexcept{return m_data[i];}
  constexpr const char&  operator[](int  i) const noexcept{return m_data[i];}

  void  print() const
  {
    printf("%c%c%c%c",m_data[0],m_data[1],m_data[2],m_data[3]);
  }

};


class
riff_subchunk_view
{
  riff_id  m_id;

  uint32_t  m_size=0;

  const uint8_t*  m_data=nullptr;

public:
  riff_subchunk_view() noexcept{}
  riff_subchunk_view(const riff_chunk&  src) noexcept{assign(src);}
  riff_subchunk_view(const uint8_t*  p) noexcept{assign(p);}

  riff_subchunk_view&  operator=(const riff_chunk&  src) noexcept;

  bool  operator==(const riff_id&  id) const noexcept{return m_id.encode() == id.encode();}

  void  assign(const riff_chunk&  src) noexcept;
  void  assign(const uint8_t*  p) noexcept;

  const riff_id&  id() const noexcept{return m_id;}

  uint32_t  size() const noexcept{return m_size;}

  const uint8_t*  data() const noexcept{return m_data;}

  void  print() const noexcept;

};


class
riff_chunk
{
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  riff_chunk() noexcept{}
  riff_chunk(const riff_chunk&   rhs) noexcept{*this = rhs;}
  riff_chunk(      riff_chunk&&  rhs) noexcept{*this = std::move(rhs);}
 ~riff_chunk(){unrefer();}

  riff_chunk&  operator=(const riff_chunk&   rhs) noexcept;
  riff_chunk&  operator=(      riff_chunk&&  rhs) noexcept;

  void  load_file(const char*  filepath) noexcept;

  uint32_t  size() const noexcept;

  const uint8_t*  data() const noexcept;

  riff_subchunk_view  subchunk_view() const noexcept{return riff_subchunk_view(*this);}

};






class
wave_format
{
  uint16_t  m_id=0;
  uint16_t  m_number_of_channels=0;
  uint32_t  m_sampling_rate=0;
  uint32_t  m_data_rate=0;
  uint16_t  m_block_size=0;
  uint16_t  m_number_of_bits_per_sample=0;

public:
  constexpr wave_format() noexcept{}

  const uint16_t&  get_id() const noexcept{return m_id;}

  void             set_sampling_rate(int  v)       noexcept{       m_sampling_rate = v;}
  const uint32_t&  get_sampling_rate(      ) const noexcept{return m_sampling_rate    ;}

  void             set_number_of_bits_per_sample(int  v)       noexcept{       m_number_of_bits_per_sample = v;}
  const uint16_t&  get_number_of_bits_per_sample(      ) const noexcept{return m_number_of_bits_per_sample    ;}

  void             set_number_of_channels(int  v)       noexcept{       m_number_of_channels = v;}
  const uint16_t&  get_number_of_channels(      ) const noexcept{return m_number_of_channels    ;}

  void  update();

  void  read_from(const riff_subchunk_view&  rv) noexcept;

  void  save_to_file(FILE*  f) const noexcept;

  void  print() const noexcept;

};


class
wave
{
  wave_format  m_format;

  uint32_t  m_length=0;

  const uint8_t*  m_data=nullptr;

  void  read_fmt( const riff_subchunk_view&  rv) noexcept;
  void  read_data(const riff_subchunk_view&  rv) noexcept;

public:
  wave() noexcept{}
  wave(const riff_subchunk_view&  rv) noexcept{assign(rv);}
  wave(const void*  data, size_t  length, const wave_format&  fmt) noexcept{assign(data,length,fmt);}

  template<typename  T>
  wave(const T*  data, size_t  length, const wave_format&  fmt) noexcept{assign(data,length,fmt);}

  wave&  assign(const riff_subchunk_view&  rv) noexcept;
  wave&  assign(const void*  data, size_t  length, const wave_format&  fmt) noexcept;

  template<typename  T>
  wave&  assign(const T*  data, size_t  length, const wave_format&  fmt) noexcept
  {
    return assign(static_cast<const void*>(data),sizeof(T)*length,fmt);
  }

  const wave_format&  get_format() const noexcept{return m_format;}

  uint32_t  length() const noexcept{return m_length;}
  uint32_t    size() const noexcept{return m_length;}

  const uint8_t*  data() const noexcept{return m_data;}

  void  save_to_file(FILE*  f) const noexcept;
  void  save_to_file(const char*  filepath) const noexcept;

  std::vector<uint8_t>  to_binary() const noexcept;

  void  print() const noexcept;

};




struct
ogg_page
{
  uint8_t  m_version=0;
  uint8_t  m_header_type=0;

  uint64_t  m_granule_position=0;

  uint32_t  m_bitstream_serial_number=0;
  uint32_t  m_sequence_number=0;
  uint32_t  m_checksum=0;

  uint8_t  m_number_of_segments=0;

  uint8_t  m_segment_table[255];

  std::vector<uint8_t>  m_segment_data;

  static bool  test(const uint8_t*  p) noexcept;

  const uint8_t*  read(const uint8_t*  p) noexcept;

  void  print() const noexcept;

};




}




#endif




