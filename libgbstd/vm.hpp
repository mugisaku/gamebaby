#ifndef gbstd_vm_HPP
#define gbstd_vm_HPP


#include"libgbstd/vms/space.hpp"


namespace gbstd{




using typesystem::type_info;
using typesystem::parameter;
using typesystem::parameter_list;
using typesystem::function_signature;
using typesystem::function_pointer_type_info;
using typesystem::pointer_type_info;
using typesystem::array_type_info;
using typesystem::struct_type_info;
using typesystem::union_type_info;


using boolean_t =  int8_t;


class
compile_error
{
public:
  int  m_line_number;

  std::string  m_comment;

  compile_error(int  ln, std::string_view  comm) noexcept: m_line_number(ln), m_comment(comm){}

};




}



#endif




