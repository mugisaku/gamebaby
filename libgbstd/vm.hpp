#ifndef gbstd_vm_HPP
#define gbstd_vm_HPP


#include"libgbstd/vms/space.hpp"


namespace gbstd{




using typesystem::type_info;
using typesystem::parameter;
using typesystem::parameter_list;
using typesystem::function_signature;


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




