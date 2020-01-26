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
append_branch_instruction(operand  cond, std::string_view  lb) noexcept
{
  m_codelines.emplace_back(*this,branch_instruction(std::move(cond),lb));

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
append_declaration(std::string_view  lb) noexcept
{
  m_declaration_list.emplace_back(lb);

  return *this;
}


function&
function::
append_declaration(const typesystem::type_info&  ti, std::string_view  lb) noexcept
{
  m_declaration_list.emplace_back(ti,lb);

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
  m_type_info->print();  

  printf("{\n");

    for(auto&  cl: m_codelines)
    {
      cl.print();

      printf("\n");
    }


  printf("}\n\n\n");
}




}




