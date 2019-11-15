#include"libgbstd/gbstd.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{



void
program_space::
load_source(std::string_view  sv) noexcept
{
  tokenizer  tknz;

  auto  root_blk = tknz(sv);

  token_block_view  bv(root_blk);

    while(bv)
    {
        if(bv[0].is_operator_code("*"))
        {
            if((bv[1].is_identifier() || bv[1].is_single_quoted() || bv[1].is_double_quoted()) &&
                bv[2].is_operator_code(",") &&
               (bv[3].is_identifier() || bv[3].is_single_quoted() || bv[3].is_double_quoted()))
            {
              m_element_table.emplace_back(asm_kind::branch,bv[1].get_string(),bv[3].get_string(),false);

              bv += 4;
            }

          else
            {
              report;
              break;
            }
        }

      else
        if(bv[0].is_identifier()    ||
           bv[0].is_single_quoted() ||
           bv[0].is_double_quoted())
        {
          auto&  s = bv[0].get_string();

            if(bv[1].is_operator_code(":"))
            {
              m_element_table.emplace_back(asm_kind::label,s,"",false);

              bv += 2;
            }

          else
            {
              m_element_table.emplace_back(asm_kind::invoke,s,"",false);

              ++bv;
            }
        }

      else
        {
          printf("[program_space error] unknown element\n");

          break;
        }
    }
}


const condition_callback*
program_space::
find_condition(std::string_view  id) const noexcept
{
    for(auto&  c: m_condition_callback_table)
    {
        if(c == id)
        {
          return &c;
        }
    }


  return nullptr;
}


const body_callback*
program_space::
find_body(std::string_view  id) const noexcept
{
    for(auto&  b: m_body_callback_table)
    {
        if(b == id)
        {
          return &b;
        }
    }


  return nullptr;
}


int
program_space::
find_label(std::string_view  id) const noexcept
{
    for(auto&  el: m_element_table)
    {
        if(el.is_label(id))
        {
          return &el-m_element_table.data();
        }
    }


  return -1;
}


void
program_space::
print() const noexcept
{
  printf("condition callback table{\n");

    for(auto&  c: m_condition_callback_table)
    {
      printf("%s\n",c.get_name().data());
    }


  printf("}\nbody callback table{\n");

    for(auto&  b: m_body_callback_table)
    {
      printf("%s\n",b.get_name().data());
    }


  printf("}\n");

  int  i = 0;

    for(auto&  el: m_element_table)
    {
      printf("%4d ",i++);

      el.print();

      printf("\n");
    }
}




}




