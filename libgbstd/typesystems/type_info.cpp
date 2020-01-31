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
make_id(const type_info&  base, pointer_type_info  ti) noexcept
{
  return base.m_id+"p";
}


std::string
type_info::
make_id(const type_info&  base, reference_type_info  ti) noexcept
{
  return base.m_id+"r";
}


std::string
type_info::
make_id(const type_info&  base, int  n) noexcept
{
  char  buf[48];

  snprintf(buf,sizeof(buf),"a%d",n);

  return base.m_id+buf;
}


std::string
type_info::
make_id(const type_info&  ret, const parameter_list&  parals) noexcept
{
  std::string  s = "f(";

    for(auto&  para: parals)
    {
      char  buf[256];

      snprintf(buf,sizeof(buf),"%s",para->get_id().data());

      s += buf;
    }


  s += ")->";
  s += ret.m_id;

  return std::move(s);
}




type_info&
type_info::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::function): m_data.parals.~parameter_list();break;
  case(kind::struct_ ): m_data.str_ti.~struct_type_info();break;
  case(kind::union_  ): m_data.uni_ti.~union_type_info();break;
  case(kind::enum_   ): m_data.enu_ti.~enum_type_info();break;
    }


  m_kind = kind::null;

  m_id.clear();

  m_number_of_elements = 0;

  return *this;
}


int
type_info::
get_size() const noexcept
{
    switch(m_kind)
    {
  case(kind::boolean):
      return m_collection.get_boolean_size();
      break;
  case(kind::null_pointer):
  case(kind::generic_pointer):
  case(kind::pointer):
  case(kind::reference):
  case(kind::function):
      return m_collection.get_pointer_size();
      break;
  case(kind::integer):
      return m_data.int_ti.get_bitwidth()/8;
      break;
  case(kind::unsigned_integer):
      return m_data.uint_ti.get_bitwidth()/8;
      break;
  case(kind::array):
      return m_base->get_size()*m_number_of_elements;
      break;
  case(kind::enum_):
      return m_collection.get_enum_size();
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
      return m_base->get_align();
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
      case(kind::pointer):
          printf("*");
          break;
      case(kind::integer):
      case(kind::unsigned_integer):
          printf("%s",tip->m_id.data());
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
            printf("function(");

              for(auto&  para: m_data.parals)
              {
                para->print();

                printf(",");
              }


            printf(")");
          }
          break;
      default:;
        }
    }


  printf(" id: \"%s\", size: %d, align: %d, ",get_id().data(),get_size(),get_align());
}




}}




