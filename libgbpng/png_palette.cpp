#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




struct
palette::
data
{
  uint32_t  m_reference_count=1;

  int  m_number_of_colors=0;

  color  m_colors[256];

};




palette::
palette(): m_data(new data)
{
}


palette&
palette::
assign(const palette&   rhs)
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

      ++m_data->m_reference_count;
    }


  return *this;
}


palette&
palette::
assign(palette&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




palette&
palette::
assign(const chunk&  chk) noexcept
{
  auto  it     = std::begin(m_data->m_colors);
  auto  it_end = std::end(  m_data->m_colors);

  auto  src = chk.get_data();

  m_data->m_number_of_colors = chk.get_data_size()/3;

    if(m_data->m_number_of_colors > 256)
    {
      m_data->m_number_of_colors = 256;
    }


    for(int  i = 0;  i < m_data->m_number_of_colors;  ++i)
    {
      it->r = *src++;
      it->g = *src++;
      it->b = *src++;

      ++it;
    }


    while(it != it_end)
    {
      it->r = 0;
      it->g = 0;
      it->b = 0;

      ++it;
    }


  return *this;
}




void
palette::
unrefer() noexcept
{
    if(m_data)
    {
      auto&  rc = m_data->m_reference_count;

        if(!--rc)
        {
          delete m_data;
        }


      m_data = nullptr;
    }
}


int
palette::
find_color(const color&  color) const noexcept
{
    for(int  i = 0;  i < m_data->m_number_of_colors;  ++i)
    {
      auto&  current = m_data->m_colors[i];

        if((current.r == color.r) &&
           (current.g == color.g) &&
           (current.b == color.b))
        {
          return i;
        }
    }


  return -1;
}


int
palette::
append_color(const color&  color) const noexcept
{
    if(m_data->m_number_of_colors < 256)
    {
        for(int  i = 0;  i < m_data->m_number_of_colors;  ++i)
        {
          auto&  current = m_data->m_colors[i];

            if((current.r == color.r) &&
               (current.g == color.g) &&
               (current.b == color.b))
            {
              return i;
            }
        }


      int  i = m_data->m_number_of_colors++;

      m_data->m_colors[i] = color;

      return i;
    }


  return -1;
}




color&  palette::get_color(int  i) const noexcept{return m_data->m_colors[i];}

int   palette::get_number_of_colors(      ) const noexcept{return m_data->m_number_of_colors    ;}
void  palette::set_number_of_colors(int  n) const noexcept{       m_data->m_number_of_colors = n;}




chunk
palette::
make_chunk() const noexcept
{
  return chunk(binary(&m_data->m_colors,3*m_data->m_number_of_colors),chunk_name("PLTE"));
}




void
palette::
print() const noexcept
{
}


}




