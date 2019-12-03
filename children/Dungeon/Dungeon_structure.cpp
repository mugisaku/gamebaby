#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




void
structure::
print() const noexcept
{
}


void
structure::
print(std::string&  sbuf) const noexcept
{
  gbstd::string_form  sf;

  sbuf += "[";

  sbuf += sf("name: \"%s\",",m_name.data());
  sbuf += sf("width: %d,",m_width);
  sbuf += sf("height: %d,",m_height);

  sbuf += "floors: [";

    for(auto&  fl: m_floors)
    {
      fl->print(sbuf);

      sbuf += ",\n";
    }


  sbuf += "]]";
}


void
structure::
scan(const gbstd::values::array&  arr) noexcept
{
  m_floors.clear();

  int  w = 0;
  int  h = 0;

  gbstd::values::array*  floors_arr = nullptr;

    for(auto&  o: arr)
    {
        if(o.test_name("name"))
        {
          m_name = o.get_string();
        }

      else
        if(o.test_name("width"))
        {
          w = o.get_integer();
        }

      else
        if(o.test_name("height"))
        {
          h = o.get_integer();
        }

      else
        if(o.test_name("floors"))
        {
          floors_arr = &o.get_array();
        }
    }


    if(floors_arr)
    {
      m_width  = w;
      m_height = h;

        for(auto&  o: *floors_arr)
        {
            if(o.is_array())
            {
report;
              m_floors.emplace_back(std::make_unique<floor>(*this,m_floors.size(),o.get_array()));
            }
        }
    }
}




}




