#include"libgbstd/vms/typesystem.hpp"
#include"libgbstd/misc.hpp"



namespace gbstd{
namespace typesystem{




type_info&
type_info::
assign(const type_info&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::basic    ): m_data.bas =      (rhs.m_data.bas);break;
      case(kind::pointer  ): m_data.ptr = clone(rhs.m_data.ptr);break;
      case(kind::reference): m_data.ref = clone(rhs.m_data.ref);break;
      case(kind::alias    ): m_data.ali = clone(rhs.m_data.ali);break;
      case(kind::array    ): m_data.arr = clone(rhs.m_data.arr);break;
      case(kind::struct_  ): m_data.st =      (rhs.m_data.st);break;
      case(kind::union_   ): m_data.un =      (rhs.m_data.un);break;
      case(kind::enum_    ): m_data.en =      (rhs.m_data.en);break;
        }
    }


  return *this;
}


type_info&
type_info::
assign(type_info&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::basic    ): m_data.bas = rhs.m_data.bas;break;
      case(kind::pointer  ): m_data.ptr = rhs.m_data.ptr;break;
      case(kind::reference): m_data.ref = rhs.m_data.ref;break;
      case(kind::alias    ): m_data.ali = rhs.m_data.ali;break;
      case(kind::array    ): m_data.arr = rhs.m_data.arr;break;
      case(kind::struct_  ): m_data.st = rhs.m_data.st;break;
      case(kind::union_   ): m_data.un = rhs.m_data.un;break;
      case(kind::enum_    ): m_data.en = rhs.m_data.en;break;
        }
    }


  return *this;
}


type_info&
type_info::
assign(basic_type_info  b) noexcept
{
  clear();

  m_kind = kind::basic;

  m_data.bas = b;

  return *this;
}


type_info&
type_info::
assign(pointer_info&&  ptr) noexcept
{
  clear();

  m_kind = kind::pointer;

  m_data.ptr = new pointer_info(std::move(ptr));

  return *this;
}


type_info&
type_info::
assign(reference_info&&  ref) noexcept
{
  clear();

  m_kind = kind::reference;

  m_data.ref = new reference_info(std::move(ref));

  return *this;
}


type_info&
type_info::
assign(alias_info&&  ali) noexcept
{
  clear();

  m_kind = kind::alias;

  m_data.ali = new alias_info(std::move(ali));

  return *this;
}


type_info&
type_info::
assign(array_info&&  arr) noexcept
{
  clear();

  m_kind = kind::array;

  m_data.arr = new array_info(std::move(arr));

  return *this;
}




void
type_info::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::pointer  ): std::destroy_at(m_data.ptr);break;
  case(kind::reference): std::destroy_at(m_data.ref);break;
  case(kind::alias    ): std::destroy_at(m_data.ali);break;
  case(kind::array    ): std::destroy_at(m_data.arr);break;
    }


  m_kind = kind::null;
}




int
type_info::
size() const noexcept
{
    switch(m_kind)
    {
  case(kind::basic    ): return m_data.bas.size();break;
  case(kind::pointer  ): return m_data.ptr->size();break;
  case(kind::reference): return m_data.ref->size();break;
  case(kind::alias    ): return m_data.ali->size();break;
  case(kind::array    ): return m_data.arr->size();break;
  case(kind::struct_): return m_data.st->size();break;
  case(kind::union_ ): return m_data.un->size();break;
  case(kind::enum_  ): return m_data.en->size();break;
    }


  return 0;
}


int
type_info::
align() const noexcept
{
    switch(m_kind)
    {
  case(kind::basic    ): return m_data.bas.size();break;
  case(kind::pointer  ): return m_data.ptr->size();break;
  case(kind::reference): return m_data.ref->size();break;
  case(kind::alias    ): return m_data.ali->align();break;
  case(kind::array    ): return m_data.arr->align();break;
  case(kind::struct_): return m_data.st->align();break;
  case(kind::union_ ): return m_data.un->align();break;
  case(kind::enum_  ): return m_data.en->size();break;
    }


  return 0;
}




type_info
type_info::
form_array(int  n) const noexcept
{
  return type_info(array_info(*this,n));
}


type_info
type_info::
form_pointer(int  sz) const noexcept
{
  return type_info(pointer_info(*this,sz));
}


type_info
type_info::
form_reference(int  sz) const noexcept
{
  return type_info(reference_info(*this,sz));
}


type_info
type_info::
remove_array() const noexcept
{
  return is_array()? get_array_info().target()
         :*this
         ;
}


type_info
type_info::
remove_pointer() const noexcept
{
  return is_pointer()? get_pointer_info().target()
         :*this
         ;
}


type_info
type_info::
remove_reference() const noexcept
{
  return is_reference()? get_reference_info().target()
         :*this
         ;
}




void
type_info::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::alias): printf("alias ");break;
  case(kind::struct_): printf("struct ");break;
  case(kind::union_ ): printf("union ");break;
  case(kind::enum_  ): printf("enum ");break;
    }


    switch(m_kind)
    {
  case(kind::alias  ): m_data.ali->print();break;
  case(kind::struct_): m_data.st->print();break;
  case(kind::union_ ): m_data.un->print();break;
  case(kind::enum_  ): m_data.en->print();break;
    }
}




}}




