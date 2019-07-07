#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




void
onch_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null):
      break;
  case(kind::cell):
      m_data.cel.~onch_cell();
      break;
  case(kind::text):
      m_data.txt.~onch_text();
      break;
  case(kind::table):
      m_data.tbl.~onch_table();
      break;
    }


  m_kind = kind::null;
}




onch_element&
onch_element::
assign(const onch_element&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::word):
          new(&m_data) onch_word(rhs.m_data.wor);
          break;
      case(kind::cell):
          new(&m_data) onch_cell(rhs.m_data.cel);
          break;
      case(kind::text):
          new(&m_data) onch_text(rhs.m_data.txt);
          break;
      case(kind::table):
          new(&m_data) onch_table(rhs.m_data.tbl);
          break;
        }
    }


  return *this;
}


onch_element&
onch_element::
assign(onch_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::null):
          break;
      case(kind::word):
          new(&m_data) onch_word(rhs.m_data.wor);
          break;
      case(kind::cell):
          new(&m_data) onch_cell(std::move(rhs.m_data.cel));
          break;
      case(kind::text):
          new(&m_data) onch_text(std::move(rhs.m_data.txt));
          break;
      case(kind::table):
          new(&m_data) onch_table(std::move(rhs.m_data.tbl));
          break;
        }
    }


  return *this;
}


onch_element&
onch_element::
assign(onch_word  wor) noexcept
{
  clear();

  m_kind = kind::word;
  new(&m_data) onch_word(std::move(wor));

  return *this;
}


onch_element&
onch_element::
assign(onch_cell&&  cel) noexcept
{
  clear();

  m_kind = kind::cell;
  new(&m_data) onch_cell(std::move(cel));

  return *this;
}


onch_element&
onch_element::
assign(onch_text&&  txt) noexcept
{
  clear();

  m_kind = kind::text;
  new(&m_data) onch_text(std::move(txt));

  return *this;
}


onch_element&
onch_element::
assign(onch_table&&  tbl) noexcept
{
  clear();

  m_kind = kind::table;
  new(&m_data) onch_table(std::move(tbl));

  return *this;
}


uint32_t
onch_element::
get_output_length() const noexcept
{
  uint32_t  l = 0;

    switch(m_kind)
    {
  case(kind::null ): break;
  case(kind::word ): l = m_data.wor.get_output_length();break;
  case(kind::cell ): l = m_data.cel.get_output_length();break;
  case(kind::text ): l = m_data.txt.get_output_length();break;
  case(kind::table): l = m_data.tbl.get_output_length();break;
    }


  return l;
}


std::vector<f32_t>
onch_element::
generate_wave(uint32_t  sampling_rate, const onch_space&  sp) const noexcept
{
  onch_output_context  ctx;

  ctx.m_sampling_rate = sampling_rate;

  ctx.m_space = &sp;

  auto  n = gbstd::sound_device::get_number_of_samples(sampling_rate,get_output_length());

  std::vector<gbstd::f32_t>  result(n);

  ctx.m_it  = result.data();
  ctx.m_end = result.data()+n;

  output(ctx);

  return std::move(result);
}


void
onch_element::
output(onch_output_context&  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::null ): break;
  case(kind::word ): m_data.wor.output(sound_kind::square_wave,ctx);break;
  case(kind::cell ): m_data.cel.output(ctx);break;
  case(kind::text ): m_data.txt.output(sound_kind::square_wave,ctx);break;
  case(kind::table): m_data.tbl.output(ctx);break;
    }
}


void
onch_element::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null ): printf("null");break;
  case(kind::word ): m_data.wor.print();break;
  case(kind::cell ): m_data.cel.print();break;
  case(kind::text ): m_data.txt.print();break;
  case(kind::table): m_data.tbl.print();break;
    }
}




}




