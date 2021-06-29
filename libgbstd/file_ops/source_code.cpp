#include"libgbstd/file_op.hpp"
#include"libgbstd/character.hpp"
#include<cinttypes>




namespace gbstd{




struct
source_code::
data
{
  std::string  m_path;

  std::u16string  m_content;

  std::vector<std::u16string_view>  m_lines;

  count_t  m_reference_count;

  data(std::string&&  path, std::u16string_view  content) noexcept{assign(std::move(path),std::u16string(content));}
  data(std::string&&  path, std::u16string&&  content) noexcept{assign(std::move(path),std::move(content));}

  data&  assign(std::string&&  path, std::u16string&&  content) noexcept;

};


source_code::data&
source_code::data::
assign(std::string&&  path, std::u16string&&  content) noexcept
{
  m_path            = std::move(   path);
  m_content         = std::move(content);
  m_reference_count =                  1;

        auto  p0 = m_content.data();
        auto  p1 = p0;
  const auto  p2 = p0+m_content.size();

  m_lines.clear();

    for(;;)
    {
        while(!(p1 == p2) && !(*p1 == u'\n'))
        {
          ++p1;
        }


      m_lines.emplace_back(p0,p1-p0);

        if(p1 == p2)
        {
          break;
        }


      p0 = ++p1;
    }


  return *this;
}




void
source_code::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->m_reference_count)
        {
          delete m_data;
        }


      m_data = nullptr;
    }
}


source_code&
source_code::
assign(const source_code&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++m_data->m_reference_count;
        }
    }


  return *this;
}


source_code&
source_code::
assign(source_code&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


source_code&
source_code::
assign(std::u16string_view  sv) noexcept
{
  unrefer();

  m_data = new data(std::string(),sv);

  return *this;
}


source_code&
source_code::
assign(std::u16string&&  s) noexcept
{
  unrefer();

  m_data = new data(std::string(),std::move(s));

  return *this;
}


const char16_t
source_code::
m_null = 0;


source_code&
source_code::
load(std::string_view  path) noexcept
{
  unrefer();

  std::string  path_s(path);

  auto  s = make_string_from_file(path_s.data());

  m_data = new data(std::move(path_s),make_u16string(s));

  return *this;
}




source_code::count_t
source_code::
reference_count() const noexcept
{
  return m_data->m_reference_count;
}


const std::string&
source_code::
path() const noexcept
{
  return m_data->m_path;
}


const std::u16string&
source_code::
content() const noexcept
{
  return m_data->m_content;
}


size_t
source_code::
size() const noexcept
{
  return m_data->m_content.size();
}


size_t
source_code::
number_of_lines() const noexcept
{
  return m_data->m_lines.size();
}


const std::u16string_view*
source_code::
begin() const noexcept
{
  return m_data->m_lines.data();
}


const std::u16string_view*
source_code::
end() const noexcept
{
  return m_data->m_lines.data()+m_data->m_lines.size();
}


void
source_code::
print() const noexcept
{
    if(m_data)
    {
      printf("[%s](%" PRIu64 "), addr:%p\n",path().data(),reference_count(),m_data);

/*
      int  i = 1;

        for(auto  sv: m_data->m_lines)
        {
          printf("%3d| ",i++);

          gbstd::print(sv);

          printf("\n");
        }
*/
    }

  else
    {
      printf("![no data]\n");
    }
}




}




