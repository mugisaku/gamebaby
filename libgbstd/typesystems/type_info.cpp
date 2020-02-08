#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




std::string
type_info::
make_id(integer_type_info  ti) noexcept
{
  char  buf[16];

  snprintf(buf,sizeof(buf),"s%d",ti.get_bitwidth());

  return buf;
}


std::string
type_info::
make_id(unsigned_integer_type_info  ti) noexcept
{
  char  buf[16];

  snprintf(buf,sizeof(buf),"u%d",ti.get_bitwidth());

  return buf;
}


std::string
type_info::
make_id(const pointer_type_info&  ti) noexcept
{
  return ti.get_base_type_info().get_id()+"p";
}


std::string
type_info::
make_id(const reference_type_info&  ti) noexcept
{
  return ti.get_base_type_info().get_id()+"r";
}


std::string
type_info::
make_id(const array_type_info&  ti) noexcept
{
  char  buf[48];

  snprintf(buf,sizeof(buf),"a%d",ti.get_number_of_elements());

  return ti.get_base_type_info().get_id()+buf;
}


std::string
type_info::
make_id(const function_signature&  fnsig) noexcept
{
  std::string  s = "f(";

    for(auto&  para: fnsig.get_parameter_list())
    {
      char  buf[256];

      snprintf(buf,sizeof(buf),"%s",para->get_id().data());

      s += buf;
    }


  s += ")->";
  s += fnsig.get_return_type_info().get_id();

  return std::move(s);
}




type_info&
type_info::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::array   ): m_data.arr_ti.~array_type_info();break;
  case(kind::struct_ ): m_data.str_ti.~struct_type_info();break;
  case(kind::union_  ): m_data.uni_ti.~union_type_info();break;
  case(kind::enum_   ): m_data.enu_ti.~enum_type_info();break;
    }


  m_kind = kind::null;

  m_id.clear();

  m_derivation->clear();

  return *this;
}


int
type_info::
get_size() const noexcept
{
    switch(m_kind)
    {
  case(kind::boolean):
      return m_data.bool_ti.get_size();
      break;
  case(kind::null_pointer):
      return m_data.nptr_ti.get_size();
      break;
  case(kind::generic_pointer):
      return m_data.gptr_ti.get_size();
      break;
  case(kind::pointer):
      return m_data.ptr_ti.get_size();
      break;
  case(kind::reference):
      return m_data.ref_ti.get_size();
      break;
  case(kind::integer):
      return m_data.int_ti.get_size();
      break;
  case(kind::unsigned_integer):
      return m_data.uint_ti.get_size();
      break;
  case(kind::array):
      return m_data.arr_ti.get_size();
      break;
  case(kind::enum_):
      return m_data.enu_ti.get_size();
      break;
  case(kind::struct_):
      return m_data.str_ti.get_size();
      break;
  case(kind::union_):
      return m_data.uni_ti.get_size();
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
    switch(m_kind)
    {
  case(kind::array):
      return m_data.arr_ti.get_align();
      break;
  case(kind::struct_):
      return m_data.str_ti.get_align();
      break;
  case(kind::union_):
      return m_data.uni_ti.get_align();
      break;
  default:
      return get_size();
    }


  return 0;
}




void
type_info::
print() const noexcept
{
/*
  std::vector<const type_info*>  buf;

  buf.emplace_back(this);

  auto  p = m_base;

    while(p)
    {
      buf.emplace_back(p);

      p = p->get_base();
    }


    while(buf.size())
    {
      auto  tip = buf.back();

      buf.pop_back();

        switch(tip->m_kind)
        {
      case(kind::void_):
          printf("void");
          break;
      case(kind::undefined):
          printf("undefined");
          break;
      case(kind::null_pointer):
          printf("nullptr_t");
          break;
      case(kind::generic_pointer):
          printf("geneptr_t");
          break;
      case(kind::pointer):
          printf("*");
          break;
      case(kind::integer):
      case(kind::unsigned_integer):
          printf("%s",tip->m_id.data());
          break;
      case(kind::boolean):
          printf("bool");
          break;
      case(kind::reference):
          printf("&");
          break;
      case(kind::array):
          printf("[%d]",tip->get_number_of_elements());
          break;
      case(kind::struct_):
          tip->m_data.str_ti.print();
          break;
      case(kind::union_):
          tip->m_data.uni_ti.print();
          break;
      case(kind::enum_):
          tip->m_data.enu_ti.print();
          break;
      case(kind::function):
          {
            printf("(*)(");

              for(auto&  para: m_data.parals)
              {
                para->print();

                printf(", ");
              }


            printf(")");
          }
          break;
      default:;
        }
    }
*/
}


void
type_info::
print_detail() const noexcept
{
  printf(" id: \"%s\", size: %d, align: %d, ",get_id().data(),get_size(),get_align());
}




}}




