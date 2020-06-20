#include"libgbstd/vms/ir.hpp"




namespace gbstd{




const ir_operation*
ir_block_info::
get_end() const noexcept
{
  return m_entry+m_number_of_operations;
}


bool
ir_block_info::
test_label(std::string_view  label) const noexcept
{
    for(auto&  lb: m_label_list)
    {
        if(lb == label)
        {
          return true;
        }
    }


  return false;
}


void
ir_block_info::
print() const noexcept
{
    for(auto&  lb: m_label_list)
    {
      printf("%s:\n",lb.data());
    }


    for(int  i = 0;  i < m_number_of_operations;  ++i)
    {
      m_entry[i].print();

      printf("\n");
    }
}




}




