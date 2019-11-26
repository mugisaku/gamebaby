#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




floor&
structure::
push_new_floor() noexcept
{
  m_floors.emplace_back(std::make_unique<floor>(*this,m_floors.size()));

  return *m_floors.back();
}


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

  sbuf += "{";

  sbuf += sf("name: \"%s\",",m_name.data());
  sbuf += sf("width: %d,",m_width);
  sbuf += sf("height: %d,",m_height);

  sbuf += "floors: [";

    for(auto&  fl: m_floors)
    {
      fl->print(sbuf);

      sbuf += ",\n";
    }


  sbuf += "]}";
}




}




