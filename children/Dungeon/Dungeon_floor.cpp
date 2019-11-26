#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




node
floor::
m_null_node;


floor::
floor(structure&  st, int  n) noexcept
{
  m_structure = &st;
  m_number    =   n;

  int  w = st.get_width() ;
  int  h = st.get_height();

  m_table.resize(w*h);

    for(int  y = 0;  y < h;  ++y){
    for(int  x = 0;  x < w;  ++x){
      m_table[(w*y)+x].assign(*this,{x,y},w,h);
    }}
}




node&
floor::
get_node(gbstd::point  pt) noexcept
{
  return m_structure->get_full_rect().test_point(pt)? m_table[(m_structure->get_width()*pt.y)+pt.x]:m_null_node;
}


const node&
floor::
get_node(gbstd::point  pt) const noexcept
{
  return m_structure->get_full_rect().test_point(pt)? m_table[(m_structure->get_width()*pt.y)+pt.x]:m_null_node;
}


bool
floor::
test_passability(gbstd::point  pt) const noexcept
{
  return m_table[(m_structure->get_width()*pt.y)+pt.x].is_passable();
}


void
floor::
print() const noexcept
{
  int  w = m_structure->get_width() ;
  int  h = m_structure->get_height();

    for(int  y = 0;  y < h;  ++y)
    {
        for(int  x = 0;  x < w;  ++x)
        {
          printf("%c",get_node({x,y}).get_symbol());
        }


      printf("\n");
    }
}


void
floor::
print(std::string&  sbuf) const noexcept
{
  int  w = m_structure->get_width() ;
  int  h = m_structure->get_height();

  sbuf += "{nodes:[";

    for(int  y = 0;  y < h;  ++y)
    {
        for(int  x = 0;  x < w;  ++x)
        {
          get_node({x,y}).print(sbuf);
        }


      sbuf += '\n';
    }


  sbuf += "]}";
}




}




