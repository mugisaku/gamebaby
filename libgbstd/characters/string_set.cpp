#include"libgbstd/character.hpp"
#include<cctype>




namespace gbstd{




string_set::section*
string_set::
get_section(std::string_view  name) noexcept
{
    for(auto&  sec: m_sections)
    {
        if(sec.m_name == name)
        {
          return &sec;
        }
    }


  return nullptr;
}


const string_set::section*
string_set::
get_section(std::string_view  name) const noexcept
{
    for(auto&  sec: m_sections)
    {
        if(sec.m_name == name)
        {
          return &sec;
        }
    }


  return nullptr;
}


int
string_set::
load_from_file(std::string_view  path) noexcept
{
  auto  s = make_string_from_file(path.data());

  return load_from_string(s);
}


namespace{
void
skip_spaces(const char*&  p) noexcept
{
    while(isspace(*p))
    {
      ++p;
    }
}
std::string
get_section_name(const char*&  p)
{
  std::string  s;

    for(;;)
    {
      auto  c = *p;

        if(c == ']')
        {
          ++p;

          break;
        }

      else
        if(isalnum(c) || (c == ' ') || (c == '_') || (c&0x80))
        {
          ++p;

          s += c;
        }

      else
        {
          report;
          break;
        }
    }


  return std::move(s);
}
std::string
get_id(const char*&  p)
{
  std::string  s;

    for(;;)
    {
      auto  c = *p;

        if(isalnum(c) || (c == ' ') || (c == '_') || (c&0x80))
        {
          ++p;

          s += c;
        }

      else
        if(c == '{')
        {
          ++p;

            if(*p == '\n')
            {
              ++p;
            }


          break;
        }

      else
        if(c == '\n')
        {
          skip_spaces(p);
        }

      else
        {
          printf("%d\n",c);

          report;
          break;
        }
    }


  return std::move(s);
}
std::u16string
get_raw_string(const char*&  p)
{
  std::u16string  s;

  utf8_decoder  dec(p);

    for(;;)
    {
      auto  c = dec();

        if(!c)
        {
          report;
          break;
        }

      else
        if(c == '}')
        {
          break;
        }

      else
        {
          s += c;
        }
    }


  p = dec.get_pointer();

  return std::move(s);
}
std::pair<std::string,std::u16string>
load_string(const char*&  p)
{
  auto  id = get_id(p);

  auto  s = get_raw_string(p);

  return std::make_pair(std::move(id),std::move(s));
}
}


int
string_set::
load_from_string(std::string_view  sv) noexcept
{
  int  n = 0;

  auto  p = sv.data();

    for(;;)
    {
      skip_spaces(p);

        if(*p == '[')
        {
          ++p;

          auto  sec_name = get_section_name(p);

          auto  sec = get_section(sec_name);

            if(!sec)
            {
              m_sections.emplace_back(std::move(sec_name));

              sec = &m_sections.back();
            }


            for(;;)
            {
              skip_spaces(p);

                if((*p == '\0') || (*p == '['))
                {
                  break;
                }


              sec->m_table.emplace_back(load_string(p));
            }
        }

      else
        {
          break;
        }
    }


  return n;
}


void
string_set::
print() const noexcept
{
    for(auto&  sec: m_sections)
    {
      printf("[%s]\n",sec.m_name.data());

        for(auto&  e: sec.m_table)
        {
          printf("%s{\n",e.first.data());

            for(auto  c: e.second)
            {
              utf8_encoder  enc(c);

              printf("%s",enc.codes);
            }


          printf("\n}\n");
        }


      printf("\n");
    }
}




std::u16string_view
string_set::section::
get(std::string_view  name) const noexcept
{
    for(auto&  e: m_table)
    {
        if(e.first == name)
        {
          return e.second;
        }
    }


  return u"[[string not found]]";
}




}




