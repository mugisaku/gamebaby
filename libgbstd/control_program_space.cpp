#include"libgbstd/gbstd.hpp"




namespace gbstd{



program_space&
program_space::
clear_image() noexcept
{
  m_image.clear();

  return *this;
}


void
program_space::
load_source(std::string_view  sv) noexcept
{
/*
  tokenizer  tknz;

  auto  root_blk = tknz(sv);

  token_iterator  it(root_blk);

    while(it)
    {
      asm_element  el;

        if(it[0].is_operator_code("*"))
        {
          ++it;

          bool  neg = false;

            if(it[0].is_operator_code("!"))
            {
              neg = true;

              ++it;
            }


            if((it[0].is_identifier() || it[0].is_single_quoted() || it[0].is_double_quoted()) &&
                it[1].is_operator_code(",") &&
               (it[2].is_identifier() || it[2].is_single_quoted() || it[2].is_double_quoted()))
            {
              el = asm_element(neg? asm_kind::jz:asm_kind::jnz,it[0].get_string(),it[2].get_string());

              it += 3;
            }

          else
            {
              report;
              break;
            }
        }

      else
        if(it[0].is_identifier()    ||
           it[0].is_single_quoted() ||
           it[0].is_double_quoted())
        {
          auto&  s = it[0].get_string();

            if(it[1].is_operator_code(":"))
            {
              el = asm_element(asm_kind::label,s,"");

              it += 2;
            }

          else
            {
              el = asm_element(asm_kind::invoke,s,"");

              ++it;
            }
        }

      else
        {
          printf("[program_space error] unknown element\n");

          break;
        }


        if(it[0].is_operator_code("."))
        {
          el.set_interrupt_flag();

          ++it;
        }


      m_image.emplace_back(el);
    }
*/
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
    for(auto&  el: m_image)
    {
        if(el.is_label(id))
        {
          return &el-m_image.data();
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

    for(auto&  el: m_image)
    {
      printf("%4d ",i++);

      el.print();

      printf("\n");
    }
}




}




