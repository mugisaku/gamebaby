#include"libgbstd/file_op.hpp"
#include"libgbstd/character.hpp"




namespace gbstd{




struct
source_code::
data
{
  std::string  m_path;

  std::u16string  m_content;

  std::vector<std::u16string_view>  m_lines;

  count_t  m_reference_count;

};


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
assign(const source_code&   rhs) noexcept
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


const char16_t
source_code::
m_null = 0;


source_code&
source_code::
load(std::string_view  path) noexcept
{
  unrefer();

  std::string  dup_path(path);

  auto  s = make_string_from_file(dup_path.data());

  m_data = new data;

  m_data->m_path            = std::move(dup_path);
  m_data->m_content         = make_u16string(s);
  m_data->m_reference_count = 1;

        auto  p0 = m_data->m_content.data();
        auto  p1 = p0;
  const auto  p2 = p0+m_data->m_content.size();

    for(;;)
    {
        while(!(p1 == p2) && !(*p1 == u'\n'))
        {
          ++p1;
        }


      m_data->m_lines.emplace_back(p0,p1-p0);

        if(p1 == p2)
        {
          break;
        }


      p0 = ++p1;
    }


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
      int  i = 1;

        for(auto  sv: m_data->m_lines)
        {
          printf("%3d| ",i++);

          gbstd::print(sv);

          printf("\n");
        }
    }
}




}




