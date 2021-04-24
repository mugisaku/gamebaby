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

  union data{
    function_pointer_type_def  fnptr_td;

    alias_type_def  ali_td;

    typed_pointer_type_def  ptr_td;
    reference_type_def      ref_td;
    array_type_def          arr_td;

    struct_type_decl   str_td;
    union_type_decl    uni_td;
    enum_type_decl     enu_td;

    data() noexcept{}
   ~data(){}

  } m_data;


  core() noexcept: m_reference_count(1){}

  void  clean(type_info::kinds  k) noexcept;

};



void
type_info::core::
clean(type_info::kinds  k)  noexcept
{
    switch(k)
    {
  case(type_info::kinds::alias           ): std::destroy_at(&m_data.ali_td);break;
  case(type_info::kinds::typed_pointer   ): std::destroy_at(&m_data.ptr_td);break;
  case(type_info::kinds::reference       ): std::destroy_at(&m_data.ref_td);break;
  case(type_info::kinds::function_pointer): std::destroy_at(&m_data.fnptr_td);break;
  case(type_info::kinds::array           ): std::destroy_at(&m_data.arr_td);break;
  case(type_info::kinds::struct_         ): std::destroy_at(&m_data.str_td);break;
  case(type_info::kinds::union_          ): std::destroy_at(&m_data.uni_td);break;
  case(type_info::kinds::enum_           ): std::destroy_at(&m_data.enu_td);break;
    }
}




type_info::type_info(alias_type_def&&             td) noexcept: m_core(new core), basic_type_info(kinds::alias           ,td.get_size(),td.get_align(),td.get_name()){create_at(&m_core->m_data.ali_td,std::move(td));}
type_info::type_info(function_pointer_type_def&&  td) noexcept: m_core(new core), basic_type_info(kinds::function_pointer,td.get_size(),""){create_at(&m_core->m_data.fnptr_td,std::move(td));}
type_info::type_info(typed_pointer_type_def&&     td) noexcept: m_core(new core), basic_type_info(kinds::typed_pointer   ,td.get_size(),""){create_at(&m_core->m_data.ptr_td,std::move(td));}
type_info::type_info(reference_type_def&&         td) noexcept: m_core(new core), basic_type_info(kinds::reference       ,td.get_size(),""){create_at(&m_core->m_data.ref_td,std::move(td));}
type_info::type_info(array_type_def&&             td) noexcept: m_core(new core), basic_type_info(kinds::array           ,td.get_size(),td.get_align(),""){create_at(&m_core->m_data.arr_td,std::move(td));}
type_info::type_info(struct_type_decl&&           td) noexcept: m_core(new core), basic_type_info(kinds::struct_         ,td.get_size(),td.get_align(),td.get_name()){create_at(&m_core->m_data.str_td,std::move(td));}
type_info::type_info(union_type_decl&&            td) noexcept: m_core(new core), basic_type_info(kinds::union_          ,td.get_size(),td.get_align(),td.get_name()){create_at(&m_core->m_data.uni_td,std::move(td));}
type_info::type_info(enum_type_decl&&             td) noexcept: m_core(new core), basic_type_info(kinds::enum_           ,td.get_size()               ,td.get_name()){create_at(&m_core->m_data.enu_td,std::move(td));}


type_info&
type_info::
assign(const type_info&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      static_cast<basic_type_info&>(*this) = static_cast<const basic_type_info&>(rhs);

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

      static_cast<basic_type_info&>(*this) = static_cast<const basic_type_info&>(rhs);

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
          m_core->clean(m_kind);

          delete m_core;
        }


      m_core = nullptr;
    }
}




const typed_pointer_type_def&  type_info::get_typed_pointer_type_def()   const noexcept{return m_core->m_data.ptr_td;}
const reference_type_def&      type_info::get_reference_type_def() const noexcept{return m_core->m_data.ref_td;}
const alias_type_def&          type_info::get_alias_type_def()     const noexcept{return m_core->m_data.ali_td;}
const array_type_def&          type_info::get_array_type_def()     const noexcept{return m_core->m_data.arr_td;}

struct_type_decl&  type_info::get_struct_type_decl() const noexcept{return m_core->m_data.str_td;}
union_type_decl&   type_info::get_union_type_decl()  const noexcept{return m_core->m_data.uni_td;}
enum_type_decl&    type_info::get_enum_type_decl()   const noexcept{return m_core->m_data.enu_td;}

const function_pointer_type_def&  type_info::get_function_pointer_type_def() const noexcept{return m_core->m_data.fnptr_td;}


void
type_info::
update_size() noexcept
{
    switch(m_kind)
    {
  case(type_info::kinds::array  ): m_size = m_core->m_data.arr_td.get_size();break;
  case(type_info::kinds::struct_): m_size = m_core->m_data.str_td->get_size();break;
  case(type_info::kinds::union_ ): m_size = m_core->m_data.uni_td->get_size();break;
  case(type_info::kinds::enum_  ): m_size = m_core->m_data.enu_td->get_size();break;
    }
}


void
type_info::
update_align() noexcept
{
    switch(m_kind)
    {
  case(type_info::kinds::array  ): m_align = m_core->m_data.arr_td.get_align();break;
  case(type_info::kinds::struct_): m_align = m_core->m_data.str_td->get_align();break;
  case(type_info::kinds::union_ ): m_align = m_core->m_data.uni_td->get_align();break;
  case(type_info::kinds::enum_  ): m_align = m_core->m_data.enu_td->get_size();break;
    }
}


void
type_info::
update() noexcept
{
   update_size();
  update_align();
}




type_info
type_info::
form_array(int  n) const noexcept
{
  return type_info(array_type_def(*this,n));
}


type_info
type_info::
form_typed_pointer(int  sz) const noexcept
{
  return type_info(typed_pointer_type_def(*this,sz));
}


type_info
type_info::
form_reference(int  sz) const noexcept
{
  return type_info(reference_type_def(*this,sz));
}


type_info
type_info::
remove_array() const noexcept
{
  return is_array()? get_array_type_def().get_base_type_info()
         :*this
         ;
}


type_info
type_info::
remove_typed_pointer() const noexcept
{
  return is_pointer()? get_typed_pointer_type_def().get_base_type_info()
         :*this
         ;
}


type_info
type_info::
remove_reference() const noexcept
{
  return is_reference()? get_reference_type_def().get_base_type_info()
         :*this
         ;
}




void
type_info::
print() const noexcept
{
    switch(m_kind)
    {
  case(type_info::kinds::alias): printf("alias ");break;
  case(type_info::kinds::struct_): printf("struct ");break;
  case(type_info::kinds::union_ ): printf("union ");break;
  case(type_info::kinds::enum_  ): printf("enum ");break;
    }


  printf("%s",m_name.data());

    switch(m_kind)
    {
  case(type_info::kinds::alias  ): m_core->m_data.ali_td.print();break;
  case(type_info::kinds::struct_): m_core->m_data.str_td.print();break;
  case(type_info::kinds::union_ ): m_core->m_data.uni_td.print();break;
  case(type_info::kinds::enum_  ): m_core->m_data.enu_td.print();break;
    }
}




}}




