#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




namespace{
bool
isnum(int  c) noexcept
{
  return (c == '?') || ((c >= '0') && (c <= '8')) ;
}


struct
decoder
{
  int  symbol;

  int   first_number;
  int  second_number;

  decoder(const char*&  ptr) noexcept;

};


decoder::
decoder(const char*&  ptr) noexcept
{
  symbol = 0;

   first_number = 0;
  second_number = 0;

    for(;;)
    {
      auto  c = *ptr;

        if(c == 0)
        {break;}

      else
        if((c == 'p') ||
           (c == 'r') ||
           (c == 'b'))
        {
          int  n = 0;

          ++ptr;

            if(sscanf(ptr," %d %n",&first_number,&n) >= 1)
            {
              symbol = c;

              ptr += n;

              break;
            }
        }

      else
        if((c == 'v') ||
           (c == 'f'))
        {
          int  n = 0;

          ++ptr;

            if(sscanf(ptr," %d -> %d %n",&first_number,&second_number,&n) >= 2)
            {
              symbol = c;

              ptr += n;

              break;
            }

          else
            if(sscanf(ptr," %d %n",&first_number,&n) >= 1)
            {
              symbol = c;

              second_number = first_number;

              ptr += n;

              break;
            }
        }


      ++ptr;
    }
}


onch_word
read_word(const char*  ptr) noexcept
{
  onch_word  w;

    while(*ptr)
    {
      decoder  dec(ptr);

        if(dec.symbol == 'p')
        {
          w.set_kind(onch_word_kind::play)
           .set_length(dec.first_number);
        }

      else
        if(dec.symbol == 'r')
        {
          w.set_kind(onch_word_kind::rest)
           .set_length(dec.first_number);
        }

      else
        if(dec.symbol == 'v')
        {
          w.set_v_level(dec.first_number,dec.second_number);
        }

      else
        if(dec.symbol == 'b')
        {
          w.set_vibrato_amount(dec.first_number);
        }

      else
        if(dec.symbol == 'f')
        {
          w.set_f_index(dec.first_number,dec.second_number);
        }

      else
        {
          break;
        }
    }


  return w;
}
}


onch_element
onch_space::
read_text(token_block_view  tbv) noexcept
{
  onch_text  txt;

    while(tbv)
    {
        if(tbv[0].is_double_quoted() || tbv[0].is_single_quoted())
        {
          auto&  s = tbv[0].get_string();

          ++tbv;

          txt.push(read_word(s.data()));
        }

      else
        if(tbv[0].is_identifier())
        {
          auto&  id = tbv[0].get_string();

          ++tbv;

          auto  e = find(id);

            if(e)
            {
                   if(e->is_text()){txt.push(e->get_text());}
              else if(e->is_word()){txt.push(e->get_word());}
            }
        }

      else
        {
          ++tbv;
        }
    }


  return onch_element(std::move(txt));
}


onch_element
onch_space::
read_cell(sound_kind  sk, token_block_view  tbv) noexcept
{
  onch_cell  cel(sk);

    while(tbv)
    {
        if(tbv[0].is_identifier())
        {
          auto&  id = tbv[0].get_string();

          auto  e = find(id);

            if(e)
            {
              ++tbv;

                   if(e->is_text()){cel.push(e->get_text());}
              else if(e->is_word()){cel.push(e->get_word());}
            }

          else
            if(tbv[1].is_block("{","}"))
            {
              auto  e = read_element(id,tbv[1].get_block());

              tbv += 2;

                if(e && e.is_text())
                {
                  cel.push(e.get_text());
                }
            }

          else
            {
              ++tbv;
            }
        }

      else
        if(tbv[0].is_single_quoted() || tbv[0].is_double_quoted())
        {
          auto&  id = tbv[0].get_string();

          ++tbv;

          auto  w = read_word(id.data());

          cel.push(w);
        }

      else
        {
          ++tbv;
        }
    }


  return onch_element(std::move(cel));
}


onch_element
onch_space::
read_table(onch_table_kind  tk, token_block_view  tbv) noexcept
{
  onch_table  tbl(tk);

    while(tbv)
    {
        if(tbv[0].is_identifier())
        {
          auto&  id = tbv[0].get_string();

          auto  e = find(id);

            if(e && (e->is_table() || e->is_cell()))
            {
              ++tbv;

              tbl.push(onch_element(*e));
            }

          else
            if(tbv[1].is_block("{","}"))
            {
              auto  e = read_element(id,tbv[1].get_block());

              tbv += 2;

                if(e)
                {
                  tbl.push(std::move(e));
                }
            }

          else
            {
              ++tbv;
            }
        }

      else
        if(tbv[0].is_single_quoted() || tbv[0].is_double_quoted())
        {
          ++tbv;
        }

      else
        {
          ++tbv;
        }
    }


  return onch_element(std::move(tbl));
}


onch_element
onch_space::
read_element(const std::string&  keyword, token_block_view  tbv) noexcept
{
       if(keyword ==      "sine"){return read_cell(sound_kind::sine_wave      ,tbv);}
  else if(keyword ==    "square"){return read_cell(sound_kind::square_wave    ,tbv);}
  else if(keyword ==    "triangle"){return read_cell(sound_kind::triangle_wave,tbv);}
  else if(keyword ==    "sawtooth"){return read_cell(sound_kind::sawtooth_wave,tbv);}
  else if(keyword ==       "noise"){return read_cell(sound_kind::noise        ,tbv);}
  else if(keyword == "short_noise"){return read_cell(sound_kind::short_noise  ,tbv);}
  else if(keyword ==         "row"){return read_table(onch_table_kind::row   ,tbv);}
  else if(keyword ==      "column"){return read_table(onch_table_kind::column,tbv);}
  else if(keyword ==        "text"){return read_text(tbv);}
  else
    {
      auto  e = find(keyword);

        if(e)
        {
          return onch_element(*e);
        }


      printf("%s is unknown keyword\n",keyword.data());
    }


  return onch_element();
}


onch_definition
onch_space::
read_define(token_block_view&  tbv) noexcept
{
    if(tbv[0].is_identifier() &&
       tbv[1].is_operator_code("="))
    {
        if(tbv[2].is_identifier() &&
           tbv[3].is_block("{","}"))
        {
          auto&       id = tbv[0].get_string();
          auto&  keyword = tbv[2].get_string();

          auto  deftbv = token_block_view(tbv[3].get_block());

          tbv += 4;

          auto  e = read_element(keyword,deftbv);

            if(e)
            {
              return onch_definition(id,std::move(e));
            }
        }

      else
        if(tbv[2].is_identifier())
        {
          auto&  id = tbv[2].get_string();

          tbv += 3;

          auto  e = find(id);

            if(e)
            {
              return onch_definition(id,onch_element(*e));
            }
        }

      else
        if(tbv[2].is_single_quoted() || tbv[2].is_double_quoted())
        {
          auto&  id = tbv[2].get_string();

          tbv += 3;

          auto  w = read_word(id.data());

          return onch_definition(id,onch_element(w));
        }

      else
        {
          tbv += 2;
        }
    }

  else
    {
      ++tbv;
    }


  return onch_definition();
}




void
onch_space::
clear() noexcept
{
  m_definition_table.clear();
}




const onch_element*
onch_space::
find(const std::string&  name) const noexcept
{
    for(auto&  def: m_definition_table)
    {
        if(def.get_name() == name)
        {
          return &def.get_element();
        }
    }


  return nullptr;
}


void
onch_space::
load_from_file(const char*  filepath) noexcept
{
  auto  s = make_string_from_file(filepath);

  load_from_string(s.data());
}


void
onch_space::
load_from_string(const char*  s) noexcept
{
  tokenizer  tknzr;

    try
    {
      auto  blk = tknzr(s);

      token_block_view  tbv(blk);

        while(tbv)
        {
          auto  def = read_define(tbv);

            if(def.get_element())
            {
              m_definition_table.emplace_back(std::move(def));
            }

          else
            {
              break;
            }
        }
    }


    catch(...)
    {
      printf("tokenizeing error\n");
    }
}


std::vector<gbstd::f32_t>
onch_space::
make_f32_raw_binary(int  sampling_rate, double  volume) const noexcept
{
  auto  e = find("main");

    if(e)
    {
      auto  buf = e->generate_wave(sampling_rate,*this);

        if(volume != 1)
        {
            for(auto&  v: buf)
            {
              v *= volume;
            }
        }


      return std::move(buf);
    }


  return {};
}


std::vector<int16_t>
onch_space::
make_16bit_raw_binary(int  sampling_rate, double  volume) const noexcept
{
  std::vector<int16_t>  buf;

  auto  e = find("main");

    if(e)
    {
      auto  wave_data = e->generate_wave(sampling_rate,*this);

      buf.resize(wave_data.size());

      auto  src     = wave_data.data();
      auto  src_end = wave_data.data()+wave_data.size();
      auto  dst     = buf.data();

        while(src != src_end)
        {
          auto  v = (*src++)*32767.0*volume;

          *dst++ = static_cast<int16_t>(v);
        }
    }


  return std::move(buf);
}


std::vector<uint8_t>
onch_space::
make_8bit_raw_binary(int  sampling_rate, double  volume) const noexcept
{
  std::vector<uint8_t>  buf;

  auto  e = find("main");

    if(e)
    {
      auto  wave_data = e->generate_wave(sampling_rate,*this);

      buf.resize(wave_data.size());

      auto  src     = wave_data.data();
      auto  src_end = wave_data.data()+wave_data.size();
      auto  dst     = buf.data();

        while(src != src_end)
        {
          auto  v = (*src++)*127.0*volume;

          *dst++ = static_cast<uint8_t>(v+128);
        }
    }


  return std::move(buf);
}


std::vector<uint8_t>
onch_space::
make_wave_format_binary(onch_sound_spec  spec) const noexcept
{
  gbstd::wave_format  fmt;

  fmt.set_sampling_rate(spec.sampling_rate);
  fmt.set_number_of_bits_per_sample(spec.bit_depth);
  fmt.set_number_of_channels(1);

  fmt.update();

    if(spec.bit_depth == 8)
    {
      auto  raw_bin = make_8bit_raw_binary(spec.sampling_rate,spec.volume);

        if(raw_bin.size())
        {
          gbstd::wave  wav(raw_bin.data(),raw_bin.size(),fmt);

          return wav.to_binary();
        }
    }

  else
    if(spec.bit_depth == 16)
    {
      auto  raw_bin = make_16bit_raw_binary(spec.sampling_rate,spec.volume);

        if(raw_bin.size())
        {
          gbstd::wave  wav(raw_bin.data(),raw_bin.size(),fmt);

          return wav.to_binary();
        }
    }


  return {};
}


void
onch_space::
print() const noexcept
{
    for(auto&  def: m_definition_table)
    {
      printf("%s = ",def.get_name().data());

      def.get_element().print();

      printf("\n");
    }


  printf("\n");
}




}




