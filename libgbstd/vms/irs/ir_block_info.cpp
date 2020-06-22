#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_block_info&
ir_block_info::
operator++() noexcept
{
  ++m_number_of_operations;

  return *this;
}




const ir_operation*
ir_block_info::
get_end() const noexcept
{
  return m_entry+m_number_of_operations;
}


void
ir_block_info::
print() const noexcept
{
  printf("%s:\n",m_label.data());

    for(int  i = 0;  i < m_number_of_operations;  ++i)
    {
      printf("  ");

      m_entry[i].print();

      printf("\n");
    }
}




}




