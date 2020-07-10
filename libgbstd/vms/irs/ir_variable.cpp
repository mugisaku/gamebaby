#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_variable&
ir_variable::
assign(const ir_value&  v)
{
       if(v->is_integer()){return assign(v.get_integer());}
  else if(v->is_fpn()    ){return assign(v.get_fpn());}
  else if(v->is_object() ){return assign(v.get_string());}

  throw ir_error("ir_variable assign error: value");
}


ir_variable&
ir_variable::
assign(int64_t  i)
{
       if(m_type_info.is_integer()){m_memory.get_content<int64_t>() = i;}
  else if(m_type_info.is_fpn())    {m_memory.get_content<double>()  = i;}
  else {print();throw ir_error("ir_variable assign error: integer");}

  return *this;
}


ir_variable&
ir_variable::
assign(double  f)
{
       if(m_type_info.is_integer()){m_memory.get_content<int64_t>() = f;}
  else if(m_type_info.is_fpn())    {m_memory.get_content<double>()  = f;}
  else {throw ir_error("ir_variable assign error: fpn");}

  return *this;
}


ir_variable&
ir_variable::
assign(std::string_view  sv)
{
    if(m_type_info.is_object())
    {
        if(m_memory.get_size() == sv.size())
        {
          std::memcpy(&m_memory[0],sv.data(),sv.size());

          return *this;
        }
    }


  throw ir_error("ir_variable assign error: object");
}




}




