#include"libgbstd/gpfs.hpp"
#include<new>




namespace gbstd{
namespace gpfs{




node::
node() noexcept:
m_name("**ROOT DIRECTORY**"),
m_kind(kind::null)
{
  m_self_reference.initialize(*this);

  be_directory();
}


node::
node(std::string_view  name) noexcept:
m_name(name),
m_kind(kind::null)
{
  m_self_reference.initialize(*this);
}




void*&
node::
be_pointer() noexcept
{
  clear();

  m_kind = kind::pointer;

  m_data.ptr = nullptr;

  return m_data.ptr;
}


int&
node::
be_integer() noexcept
{
  clear();

  m_kind = kind::integer;

  m_data.i = 0;

  return m_data.i;
}


double&
node::
be_real_number() noexcept
{
  clear();

  m_kind = kind::real_number;

  m_data.d = 0;

  return m_data.d;
}


callback_wrapper&
node::
be_callback() noexcept
{
  clear();

  m_kind = kind::callback;

  new(&m_data.cb) callback_wrapper();

  return m_data.cb;
}


sprite&
node::
be_sprite() noexcept
{
  clear();

  m_kind = kind::sprite;

  new(&m_data) sprite();

  return m_data.spr;
}


timer&
node::
be_timer() noexcept
{
  clear();

  m_kind = kind::timer;

  new(&m_data) timer();

  return m_data.tmr;
}


clock&
node::
be_clock() noexcept
{
  clear();

  m_kind = kind::clock;

  new(&m_data) clock();

  return m_data.clk;
}


directory&
node::
be_directory() noexcept
{
  clear();

  m_kind = kind::directory;

  new(&m_data) directory();

  m_data.dir.m_self_node = this;

  return m_data.dir;
}


node_reference&
node::
be_reference() noexcept
{
  clear();

  m_kind = kind::reference;

  new(&m_data) node_reference();

  return m_data.ref;
}




void
node::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::directory): m_data.dir.~directory();break;
  case(kind::reference): m_data.ref.~node_reference();break;
  case(kind::callback ): m_data.cb.~callback_wrapper();break;
  case(kind::sprite   ): m_data.spr.~sprite();break;
  case(kind::timer    ): m_data.tmr.~timer();break;
  case(kind::clock    ): m_data.clk.~clock();break;

  default:;
    }


  m_kind = kind::null;
}


void
node::
print() const noexcept
{
  const char*  type_s = is_directory()?   "directory"
                       :is_pointer()?     "pointer"
                       :is_reference()?   "reference"
                       :is_integer()?     "integer"
                       :is_real_number()? "real_number"
                       :is_callback()?    "callback"
                       :is_sprite()?      "sprite"
                       :is_timer()?       "timer"
                       :is_clock()?       "clock"
                       :                  "null";

  printf("name: \"%s\", type: %s, data: ",m_name.data(),type_s);

    switch(m_kind)
    {
  case(kind::null): printf("null");break;
  case(kind::directory):
      printf("{\n");

      m_data.dir.print();

      printf("\n}\n");
    break;
  case(kind::reference):
      m_data.ref.print();
    break;
  case(kind::integer): printf("%d",m_data.i);break;
  case(kind::real_number): printf("%f",m_data.d);break;

  default:;
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




