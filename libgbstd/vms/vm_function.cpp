#include"libgbstd/vm.hpp"




namespace gbstd{




codeline&
function::
create_codeline() noexcept
{
  m_codelines.emplace_back(*this);

  return m_codelines.back();
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
  printf("function %s\n{\n",m_name.data());

    for(auto&  cl: m_codelines)
    {
      cl.print();

      printf("\n");
    }


  printf("}\n\n\n");
}




}




