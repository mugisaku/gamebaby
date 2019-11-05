#include"libgbstd/gpfs.hpp"
#include<new>




namespace gbstd{
namespace gpfs{




struct
node_reference::
data
{
  node*     m_pointer;
  uint32_t  m_counter;

};


void
node_reference::
unrefer() noexcept
{
    if(m_data)
    {
      auto&  n = m_data->m_counter;

        if(!--n)
        {
          delete m_data;
        }


      m_data = nullptr;
    }
}


void
node_reference::
unpoint() noexcept
{
    if(m_data)
    {
      m_data->m_pointer = nullptr;
    }
}


void
node_reference::
initialize(node&  nd) noexcept
{
  m_data = new data;

  m_data->m_pointer = &nd;
  m_data->m_counter =   1;
}


node_reference::operator bool() const noexcept{return m_data && m_data->m_pointer;}


node&  node_reference::operator*()  const noexcept{return *m_data->m_pointer;}
node*  node_reference::operator->() const noexcept{return  m_data->m_pointer;}


node_reference&
node_reference::
assign(const node&  nd) noexcept
{
  return assign(nd.get_self_reference());
}


node_reference&
node_reference::
assign(const node_reference&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

      m_data->m_counter += 1;
    }


  return *this;
}


node_reference&
node_reference::
assign(node_reference&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


void
node_reference::
print() const noexcept
{
    if(m_data)
    {
      printf("pointer is %svalid, ",m_data->m_pointer? "":"in");

        if(*this)
        {
          m_data->m_pointer->print();
        }
    }

  else
    {
      printf("empty");
    }
}




}}




