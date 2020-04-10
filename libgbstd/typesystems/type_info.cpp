#include"libgbstd/typesystem.hpp"
#include"libgbstd/misc.hpp"



namespace gbstd{
namespace typesystem{




class
type_info::
core
{
public:
  int  m_reference_count;

  kind  m_kind;

  union data{
             integer_type_info   int_ti;
    unsigned_integer_type_info  uint_ti;
                 fpn_type_info   fpn_ti;

    boolean_type_info           bool_ti;
    generic_pointer_type_info   gptr_ti;
    pointer_type_info            ptr_ti;
    reference_type_info          ref_ti;

    array_type_info    arr_ti;
    struct_type_info   str_ti;
    union_type_info    uni_ti;
    enum_type_info     enu_ti;

    function_pointer_type_info  fnptr_ti;

    data() noexcept{}
   ~data(){}

  } m_data;


  std::string  m_name;

  std::string  m_id;

  core(kind  k, std::string&&  id) noexcept: m_reference_count(1), m_kind(k), m_id(std::move(id)){}
 ~core();

};



type_info::core::
~core()
{
    switch(m_kind)
    {
  case(kind::pointer         ): std::destroy_at(&m_data.ptr_ti);break;
  case(kind::reference       ): std::destroy_at(&m_data.ref_ti);break;
  case(kind::function_pointer): std::destroy_at(&m_data.fnptr_ti);break;
  case(kind::array           ): std::destroy_at(&m_data.arr_ti);break;
  case(kind::struct_         ): std::destroy_at(&m_data.str_ti);break;
  case(kind::union_          ): std::destroy_at(&m_data.uni_ti);break;
  case(kind::enum_           ): std::destroy_at(&m_data.enu_ti);break;
    }
}




type_info::type_info(void_type_info)                   noexcept: m_core(new core(kind::void_           ,"void")     ){}
type_info::type_info(undefined_type_info)              noexcept: m_core(new core(kind::undefined       ,"undefined")){}
type_info::type_info(null_pointer_type_info)           noexcept: m_core(new core(kind::null_pointer    ,"nullptr")  ){}
type_info::type_info(boolean_type_info&&           ti) noexcept: m_core(new core(kind::boolean         ,"bool")      ){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(generic_pointer_type_info&&   ti) noexcept: m_core(new core(kind::generic_pointer ,"geneptr")   ){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(         integer_type_info&&  ti) noexcept: m_core(new core(kind::integer         ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(unsigned_integer_type_info&&  ti) noexcept: m_core(new core(kind::unsigned_integer,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(fpn_type_info&&               ti) noexcept: m_core(new core(kind::fpn             ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(pointer_type_info&&           ti) noexcept: m_core(new core(kind::pointer         ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(reference_type_info&&         ti) noexcept: m_core(new core(kind::reference       ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(struct_type_info&&            ti) noexcept: m_core(new core(kind::struct_         ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(union_type_info&&             ti) noexcept: m_core(new core(kind::union_          ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(enum_type_info&&              ti) noexcept: m_core(new core(kind::enum_           ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info(array_type_info&&             ti) noexcept: m_core(new core(kind::array           ,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}
type_info::type_info::type_info(function_pointer_type_info&&  ti) noexcept: m_core(new core(kind::function_pointer,ti.make_id())){create_at(&m_core->m_data,std::move(ti));}




type_info::kind
type_info::
get_kind() const noexcept
{
  return m_core->m_kind;
}


type_info&
type_info::
assign(const type_info&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_core = rhs.m_core;

        if(m_core)
        {
          ++(m_core->m_reference_count);
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
      unrefer();

      std::swap(m_core,rhs.m_core);
    }


  return *this;
}




void
type_info::
unrefer() noexcept
{
    if(m_core)
    {
      auto&  c = m_core->m_reference_count;

        if(!--c)
        {
          delete m_core;
        }


      m_core = nullptr;
    }
}


void  type_info::set_name(std::string_view  name) noexcept{m_core->m_name = name;}

const std::string&  type_info::get_name() const noexcept{return m_core->m_name;}
const std::string&  type_info::get_id() const noexcept{return m_core->m_id;}


int
type_info::
get_size() const noexcept
{
    switch(get_kind())
    {
  case(kind::boolean):
      return m_core->m_data.bool_ti.get_size();
      break;
  case(kind::generic_pointer):
      return m_core->m_data.gptr_ti.get_size();
      break;
  case(kind::function_pointer):
      return m_core->m_data.fnptr_ti.get_size();
      break;
  case(kind::pointer):
      return m_core->m_data.ptr_ti.get_size();
      break;
  case(kind::reference):
      return m_core->m_data.ref_ti.get_size();
      break;
  case(kind::integer):
      return m_core->m_data.int_ti.get_size();
      break;
  case(kind::unsigned_integer):
      return m_core->m_data.uint_ti.get_size();
      break;
  case(kind::fpn):
      return m_core->m_data.fpn_ti.get_size();
      break;
  case(kind::array):
      return m_core->m_data.arr_ti.get_size();
      break;
  case(kind::enum_):
      return m_core->m_data.enu_ti.get_size();
      break;
  case(kind::struct_):
      return m_core->m_data.str_ti.get_size();
      break;
  case(kind::union_):
      return m_core->m_data.uni_ti.get_size();
      break;
  default:
      return 0;
    }


  return 0;
}


int
type_info::
get_align() const noexcept
{
    switch(get_kind())
    {
  case(kind::array):
      return m_core->m_data.arr_ti.get_align();
      break;
  case(kind::struct_):
      return m_core->m_data.str_ti.get_align();
      break;
  case(kind::union_):
      return m_core->m_data.uni_ti.get_align();
      break;
  default:
      return get_size();
    }


  return 0;
}




const          integer_type_info&           type_info::get_integer_type_info() const noexcept{return m_core->m_data.int_ti;}
const unsigned_integer_type_info&  type_info::get_unsigned_integer_type_info() const noexcept{return m_core->m_data.uint_ti;}

const fpn_type_info&  type_info::get_fpn_type_info() const noexcept{return m_core->m_data.fpn_ti;}

const   pointer_type_info&    type_info::get_pointer_type_info() const noexcept{return m_core->m_data.ptr_ti;}
const reference_type_info&  type_info::get_reference_type_info() const noexcept{return m_core->m_data.ref_ti;}

const array_type_info&    type_info::get_array_type_info() const noexcept{return m_core->m_data.arr_ti;}
const struct_type_info&  type_info::get_struct_type_info() const noexcept{return m_core->m_data.str_ti;}
const union_type_info&    type_info::get_union_type_info() const noexcept{return m_core->m_data.uni_ti;}
const enum_type_info&      type_info::get_enum_type_info() const noexcept{return m_core->m_data.enu_ti;}

const function_pointer_type_info&  type_info::get_function_pointer_type_info() const noexcept{return m_core->m_data.fnptr_ti;}




type_info
type_info::
form_array_type(int  n) const noexcept
{
  return type_info(array_type_info(*this,n));
}


type_info
type_info::
form_reference_type(int  w) const noexcept
{
    if(is_reference())
    {
      return *this;
    }



   return type_info(reference_type_info(*this,w));
}


type_info
type_info::
form_pointer_type(int  w) const noexcept
{
   return type_info(pointer_type_info(*this,w));
}




type_info
type_info::
strip_pointer_type() const noexcept
{
  return is_pointer()? get_pointer_type_info().get_base_type_info()
        :*this;
}


type_info
type_info::
strip_reference_type() const noexcept
{
  return is_reference()? get_reference_type_info().get_base_type_info()
        :*this;
}


type_info
type_info::
strip_array_type() const noexcept
{
  return is_array()? get_array_type_info().get_base_type_info()
        :*this;
}




void
type_info::
print() const noexcept
{
  printf("%s",m_core->m_name.data());
}


void
type_info::
print_detail() const noexcept
{
  printf(" id: \"%s\", size: %d, align: %d, ",get_id().data(),get_size(),get_align());
}




}}




