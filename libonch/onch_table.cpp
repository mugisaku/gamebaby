#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




void
onch_table::
push(onch_element&&  e) noexcept
{
  m_elements.emplace_back(std::move(e));
}


uint32_t
onch_table::
get_output_length() const noexcept
{
  uint32_t  l = 0;

    if(is_row())
    {
        for(auto&  e: m_elements)
        {
          l += e.get_output_length();
        }
    }

  else
    if(is_column())
    {
        for(auto&  e: m_elements)
        {
          l = std::max(l,e.get_output_length());
        }
    }


  return l;
}




void
onch_table::
output(onch_output_context&  ctx) const noexcept
{
    if(is_row())
    {
        for(auto&  e: m_elements)
        {
            if(e.is_cell())
            {
              e.get_cell().output(ctx);
            }

          else
            if(e.is_table())
            {
              e.get_table().output(ctx);
            }
        }
    }

  else
    if(is_column())
    {
      auto  it = ctx.m_it;

      auto  max_it = it;

        for(auto&  e: m_elements)
        {
            if(e.is_cell())
            {
              e.get_cell().output(ctx);
            }

          else
            if(e.is_table())
            {
              e.get_table().output(ctx);
            }


          max_it = std::max(max_it,ctx.m_it);

          ctx.m_it = it;
        }


      ctx.m_it = max_it;
    }
}


void
onch_table::
print() const noexcept
{
    switch(m_kind)
    {
  case(onch_table_kind::row):
      printf("row{");

        for(auto&  e: m_elements)
        {
          e.print();

          printf(" ");
        }

      printf("}");
      break;
  case(onch_table_kind::column):
      printf("column{");

        for(auto&  e: m_elements)
        {
          e.print();

          printf("\n");
        }

      printf("}");
      break;
    }
}




}




