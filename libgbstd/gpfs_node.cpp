#include"libgbstd/gpfs.hpp"
#include<new>




namespace gbstd{
namespace gpfs{




node::
node() noexcept:
m_parent(nullptr),
m_name("**ROOT DIRECTORY**"),
m_kind(kind::null)
{
  be_directory();
}


node::
node(node&  parent, std::string_view  name) noexcept:
m_parent(&parent),
m_name(name),
m_kind(kind::null)
{
}




node&
node::
be_pointer(void*  ptr) noexcept
{
  clear();

  m_kind = kind::pointer;

  m_data.ptr = ptr;

  return *this;
}


node&
node::
be_integer(int  i) noexcept
{
  clear();

  m_kind = kind::integer;

  m_data.i = i;

  return *this;
}


node&
node::
be_real_number(double  d) noexcept
{
  clear();

  m_kind = kind::real_number;

  m_data.d = d;

  return *this;
}


node&
node::
be_callback(callback_wrapper  cb) noexcept
{
  clear();

  m_kind = kind::callback;

  new(&m_data.cb) callback_wrapper(cb);

  return *this;
}


node&
node::
be_directory() noexcept
{
  clear();

  m_kind = kind::directory;

  new(&m_data) directory();

  m_data.dir.m_self_node = this;

  return *this;
}




void
node::
clear() noexcept
{
    if(is_directory())
    {
      m_data.dir.~directory();
    }

  else
    if(is_callback())
    {
      m_data.cb.~callback_wrapper();
    }


  m_kind = kind::null;
}


void
node::
print() const noexcept
{
  const char*  type_s = is_directory()?   "directory"
                       :is_pointer()?     "pointer"
                       :is_integer()?     "integer"
                       :is_real_number()? "real_number"
                       :is_callback()?    "callback"
                       :                  "null";

  printf("name: \"%s\", type: %s,",m_name.data(),type_s);

    if(is_directory())
    {
      printf("{\n");

      get_directory().print();

      printf("\n}\n");
    }
}


void
node::
hook(node*  a, node*  b) noexcept
{
    if(a){a->m_next     = b;}
    if(b){b->m_previous = a;}
}


void
node::
unhook() noexcept
{
    if(m_previous){m_previous->m_next =     m_next;}
    if(m_next    ){m_next->m_previous = m_previous;}
}




}}




