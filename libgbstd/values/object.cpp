#include"libgbstd/value.hpp"
#include"libgbstd/character.hpp"




namespace gbstd{
namespace values{




object&
object::
assign(bool   b) noexcept
{
  clear();

  new(&m_data) bool(b);

  m_kind = kind::boolean;

  return *this;
}


object&
object::
assign(int  i) noexcept
{
  clear();

  new(&m_data) int(i);

  m_kind = kind::integer;

  return *this;
}


object&
object::
assign(std::string_view  s) noexcept
{
  clear();

  new(&m_data) std::string(s);

  m_kind = kind::string;

  return *this;
}


object&
object::
assign(std::u16string_view  u16s) noexcept
{
  clear();

  new(&m_data) std::u16string(u16s);

  m_kind = kind::u16string;

  return *this;
}


object&
object::
assign(array&&  arr) noexcept
{
  clear();

  m_data.arr = new array(std::move(arr));

  m_kind = kind::array;

  return *this;
}


object&
object::
assign(list&&  ls) noexcept
{
  clear();

  m_data.ls = new list(std::move(ls));

  m_kind = kind::list;

  return *this;
}


object&
object::
assign(const object&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_has_name = rhs.m_has_name;
      m_kind     = rhs.m_kind;
      m_name     = rhs.m_name;

        switch(m_kind)
        {
      case(kind::boolean): m_data.b = rhs.m_data.b;break;
      case(kind::integer): m_data.i = rhs.m_data.i;break;
      case(kind::string ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::u16string ): new(&m_data) std::u16string(rhs.m_data.u16s);break;
      case(kind::array): m_data.arr = new array(*rhs.m_data.arr);break;
      case(kind::list): m_data.ls = new list(*rhs.m_data.ls);break;
        }
    }


  return *this;
}


object&
object::
assign(object&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();


      std::swap(m_has_name,rhs.m_has_name);
      std::swap(m_kind,rhs.m_kind);
      std::swap(m_name,rhs.m_name);

        switch(m_kind)
        {
      case(kind::boolean  ): m_data.b = rhs.m_data.b;break;
      case(kind::integer  ): m_data.i = rhs.m_data.i;break;
      case(kind::string   ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::u16string): new(&m_data) std::u16string(std::move(rhs.m_data.u16s));break;
      case(kind::array    ): m_data.arr = rhs.m_data.arr;break;
      case(kind::list     ): m_data.ls = rhs.m_data.ls;break;
        }
    }


  return *this;
}




void
object::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::undefined): break;
  case(kind::boolean): break;
  case(kind::integer): break;
  case(kind::string ): gbstd::destruct(m_data.s);break;
  case(kind::u16string): gbstd::destruct(m_data.u16s);break;
  case(kind::array): delete m_data.arr;break;
  case(kind::list): delete m_data.ls;break;
    }


  m_kind = kind::null;

  unset_name();
}


object&
object::
set_name(std::string_view  name) noexcept
{
  m_has_name = true;

  m_name = name;

  return *this;
}


object&
object::
unset_name() noexcept
{
  m_has_name = false;

  m_name.clear();

  return *this;
}


void
object::
print() const noexcept
{
    if(has_name())
    {
      printf("%s:",m_name.data());
    }


    switch(m_kind)
    {
  case(kind::undefined): printf("undefined");break;
  case(kind::null): printf("null");break;
  case(kind::boolean): printf("%s",m_data.b? "true":"false");break;
  case(kind::integer): printf("%d",m_data.i);break;
  case(kind::string ): printf("\"%s\"",m_data.s.data());break;
  case(kind::u16string ):
    printf("u\"");
    gbstd::print(m_data.u16s);
    printf("\"");
    break;
  case(kind::list): m_data.ls->print();break;
  case(kind::array): m_data.arr->print();break;
  default: printf("unknown");break;
    }
}


const object  null      = object::make_null();
const object  undefined = object::make_undefined();


}}




