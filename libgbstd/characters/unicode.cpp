#include"libgbstd/character.hpp"
#include<cstdio>




namespace gbstd{




size_t
u8clen(unsigned char  c) noexcept
{
       if(!(c>>7)             ){return 1;}
  else if( (c>>1) == 0b1111110){return 6;}
  else if( (c>>2) == 0b111110 ){return 5;}
  else if( (c>>3) == 0b11110  ){return 4;}
  else if( (c>>4) == 0b1110   ){return 3;}
  else if( (c>>5) == 0b110    ){return 2;}
  else if( (c>>6) == 0b10     ){return 1;}

  printf("%dはUTF8の先頭ではありません\n",c);

  return 0;
}


size_t
u8slen(const char*  s) noexcept
{
  size_t  len = 0;

  utf8_decoder  dec(s);

    while(dec)
    {
      dec();

      ++len;
    }


  return len;
}


std::u16string
make_u16string(std::string_view  sv) noexcept
{
  std::u16string  u16s;

  utf8_decoder  dec(sv.data());

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      u16s += c;
    }


  return std::move(u16s);
}




namespace{
int
decode(int  c, int  shift_amount=0) noexcept
{
  return((c&0b111111)<<shift_amount);
}
int
encode(int  c, int  shift_amount=0) noexcept
{
  return((c>>shift_amount)&0b111111);
}
}


utf8_decoder&
utf8_decoder::
assign(std::string_view  sv) noexcept
{
  m_pointer = sv.data();
  m_end     = sv.data()+sv.size();

  return *this;
}


char32_t
utf8_decoder::
operator()() noexcept
{
    if(!*m_pointer)
    {
      return 0;
    }


  char32_t  c = 0;

  const auto  n = u8clen(*m_pointer);


  auto  p = reinterpret_cast<const uint8_t*>(m_pointer);

    switch(n)
    {
  case(1): c = ((p[0]             )            )               ;break;
  case(2): c = ((p[0]&0b11111)<< 6)|decode(p[1])               ;break;
  case(3): c = ((p[0]&0b01111)<<12)|decode(p[1],6)|decode(p[2]);break;

  case(4):
    c = ((p[0]&0b111)<<18)|decode(p[1],12)|
                           decode(p[2], 6)|
                           decode(p[3]   );
    break;
  case(5):
    c = ((p[0]&0b11)<<24)|decode(p[1],18)|
                          decode(p[2],12)|
                          decode(p[3], 6)|
                          decode(p[4]   );
    break;
  case(6):
    c = ((p[0]&0b1)<<30)|decode(p[1],24)|
                         decode(p[2],18)|
                         decode(p[3],12)|
                         decode(p[4], 6)|
                         decode(p[5]   );
    break;
  default:
      printf("不正なUTF8のバイト数です(%ld)\n",n);
      return 0;
    }


  m_pointer += n;

  return c;
}




utf8_encoder&
utf8_encoder::
operator()(char32_t  c) noexcept
{
  char*  p = codes;

    if(c <= 0x7F)
    {
      *p++ = c;
    }

  else
    if((c >= 0x0080) &&
       (c <= 0x07FF))
    {
      *p++ = (0b11000000|(c>>6));
      *p++ = (0b10000000|encode(c));
    }

  else
    if((c >= 0x0800) &&
       (c <= 0xFFFF))
    {
      *p++ = (0b11100000|(c>>12));
      *p++ = (0b10000000|encode(c,6));
      *p++ = (0b10000000|encode(c  ));
    }

  else
    if((c >= 0x010000) &&
       (c <= 0x1FFFFF))
    {
      *p++ = (0b11110000|(c>>18));
      *p++ = (0b10000000|encode(c,12));
      *p++ = (0b10000000|encode(c, 6));
      *p++ = (0b10000000|encode(c   ));
    }

  else
    if((c >= 0x0200000) &&
       (c <= 0x3FFFFFF))
    {
      *p++ = (0b11111000|(c>>24));
      *p++ = (0b10000000|encode(c,18));
      *p++ = (0b10000000|encode(c,12));
      *p++ = (0b10000000|encode(c, 6));
      *p++ = (0b10000000|encode(c   ));
    }

  else
    if((c >= 0x04000000) &&
       (c <= 0x7FFFFFFF))
    {
      *p++ = (0b11111100|(c>>30));
      *p++ = (0b10000000|encode(c,24));
      *p++ = (0b10000000|encode(c,18));
      *p++ = (0b10000000|encode(c,12));
      *p++ = (0b10000000|encode(c, 6));
      *p++ = (0b10000000|encode(c   ));
    }

  else
    {
      printf("%dはユニコードではありません\n",c);
    }


  *p = 0;

  return *this;
}


void
print(std::u16string_view  sv) noexcept
{
  utf8_encoder  enc;

    for(auto  u16: sv)
    {
      printf("%s",enc(u16).codes);
    }
}


void
print_nl() noexcept
{
  printf("\n");
}




code_text&
code_text::
append(std::string_view  sv) noexcept
{
  m_string = make_u16string(sv);

  auto  p = m_string.data();

  m_line_heads.emplace_back(p);

    while(*p)
    {
      auto  c = *p++;

        if(c == '\n')
        {
          m_line_heads.emplace_back(p);
        }
    }


  return *this;
}


code_text&
code_text::
assign(std::string_view  sv) noexcept
{
  m_line_heads.clear();

  return append(sv);
}


void
code_text::
print() const noexcept
{
  gbstd::print(m_string);

  printf("\n");
}




code_text::iterator&
code_text::iterator::
operator++() noexcept
{
  auto  c = *m_pointer++;

    if(c == '\n')
    {
      m_x_index  = 0;
      m_y_index += 1;
    }

  else
    {
      ++m_x_index;
    }


  return *this;
}


void
code_text::iterator::
print() const noexcept
{
  printf("{%4d行目, %4d文字目}",1+m_y_index,1+m_x_index);
}


void
code_text::iterator::
skip_spaces() noexcept
{
    for(;;)
    {
      auto  c = **this;

        if((c == ' ') ||
           (c == '\t'))
        {
          ++m_pointer;
          ++m_x_index;
        }

      else
        if(c == '\n')
        {
          ++m_pointer;

          m_x_index  = 0;
          m_y_index += 1;
        }

      else
        {
          break;
        }
    }
}




}




