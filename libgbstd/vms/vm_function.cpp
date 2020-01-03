#include"libgbstd/vm.hpp"




namespace gbstd{




function&
function::
append_store_instruction(operand  dst, operand  src) noexcept
{
  m_codelines.emplace_back(*this,store_instruction(std::move(dst),std::move(src)));

  return *this;
}


function&
function::
append_branch_instruction(operand  cond, std::string_view  nz, std::string_view  z) noexcept
{
  m_codelines.emplace_back(*this,branch_instruction(std::move(cond),nz,z));

  return *this;
}


function&
function::
append_return_instruction(operand  o) noexcept
{
  m_codelines.emplace_back(*this,return_instruction(std::move(o)));

  return *this;
}


function&
function::
append_operation(operation  op) noexcept
{
  m_codelines.emplace_back(*this,std::move(op));

  return *this;
}




function&
function::
append_variable_initializer(std::string_view  lb, operand&&  o) noexcept
{
  m_variable_initializer_list.emplace_back(lb,std::move(o));

  return *this;
}


function&
function::
append_parameter(std::string_view  lb) noexcept
{
  m_parameter_list.emplace_back(lb);

  return *this;
}


function&
function::
append_entry_point(std::string_view  lb) noexcept
{
  m_entry_point_list.emplace_back(lb,m_codelines.size());

  return *this;
}


const operation*
function::
find_operation(std::string_view  label) const noexcept
{
    for(auto&  cl: m_codelines)
    {
        if(cl.is_operation())
        {
          auto&  op = cl.get_operation();

            if(op.get_label() == label)
            {
              return &op;
            }
        }
    }


  return nullptr;
}


const entry_point*
function::
find_entry_point(std::string_view  label) const noexcept
{
    for(auto&  ep: m_entry_point_list)
    {
        if(ep.get_label() == label)
        {
          return &ep;
        }
    }


  return nullptr;
}


void
function::
print() const noexcept
{
  printf("function %s(",m_name.data());

    for(auto&  p: m_parameter_list)
    {
      printf("%s, ",p.data());
    }


  printf(")\n{\n");

    for(auto&  cl: m_codelines)
    {
      cl.print();

      printf("\n");
    }


  printf("}\n\n\n");
}




}




