#include"libgbstd/vms/ir.hpp"




namespace gbstd{




namespace{
ir_operand
read_operand(token_iterator&  it)
{
    if(it->is_integer())
    {
      return ir_operand(static_cast<int64_t>(it++->get_integer()));
    }

  else
    if(it->is_operator_code("-"))
    {
      ++it;

        if(it->is_integer())
        {
          return ir_operand(-static_cast<int64_t>(it++->get_integer()));
        }

      else
        if(it->is_floating_point_number())
        {
          return ir_operand(-it++->get_floating_point_number());
        }
    }

  else
    if(it->is_floating_point_number())
    {
      return ir_operand(it++->get_floating_point_number());
    }

  else
    if(it->is_identifier())
    {
      return ir_operand(it++->get_string());
    }

  else
    if(it->is_string())
    {
      auto&  s = it++->get_string();

      return ir_operand(s.data(),s.size());
    }


  return ir_operand();
}


std::vector<ir_phi_element>
read_phi_element_list(token_iterator&  it)
{
  std::vector<ir_phi_element>  ls;

    for(;;)
    {
        if(!it)
        {
          throw ir_error("read phi error");
        }

      else
        if(it->is_operator_code("]"))
        {
          ++it;

          break;
        }

      else
        if(it->is_identifier())
        {
          auto&  label = it++->get_string();

            if(it->is_operator_code(":"))
            {
              ++it;
            }


          auto  o = read_operand(it);

          ls.emplace_back(label,std::move(o));

            if(it->is_operator_code(","))
            {
              ++it;
            }
        }

      else
        {
          throw ir_error("read phi error: invalid element");
        }
    }


  return std::move(ls);
}


std::vector<ir_operand>
read_operand_list(token_iterator&  it)
{
  std::vector<ir_operand>  ls;

    for(;;)
    {
        if(!it)
        {
          throw ir_error("ir_operation read error: invalid cal");
        }

      else
        if(it->is_operator_code(")"))
        {
          ++it;

          break;
        }

      else
        {
          ls.emplace_back(read_operand(it));

            if(it->is_operator_code(","))
            {
              ++it;
            }
        }
    }


  return std::move(ls);
}
}


void
ir_operation::
read_assign(std::string_view  sv, token_iterator&  it)
{
    if(it->is_operator_code("="))
    {
      ++it;
    }


    if(it->is_identifier())
    {
      set_label(sv);

      auto&  instr = it++->get_string();

        if(instr == std::string_view("cal"))
        {
          m_instruction = instr;

            if(it->is_identifier())
            {
              m_operand_list.emplace_back(it++->get_string());
            }


            if(it->is_operator_code("("))
            {
              m_operand_list.emplace_back(read_operand_list(++it));
            }
        }

      else
        if((instr == std::string_view("add")) ||
           (instr == std::string_view("sub")) ||
           (instr == std::string_view("mul")) ||
           (instr == std::string_view("div")) ||
           (instr == std::string_view("rem")) ||
           (instr == std::string_view("shl")) ||
           (instr == std::string_view("shr")) ||
           (instr == std::string_view("bit_or")) ||
           (instr == std::string_view("bit_and")) ||
           (instr == std::string_view("bit_xor")) ||
           (instr == std::string_view("eq")) ||
           (instr == std::string_view("neq")) ||
           (instr == std::string_view("lt")) ||
           (instr == std::string_view("lteq")) ||
           (instr == std::string_view("gt")) ||
           (instr == std::string_view("gteq")) ||
           (instr == std::string_view("log_or")) ||
           (instr == std::string_view("log_and")))
        {
          m_instruction = instr;

          auto  lo = read_operand(it);
          auto  ro = read_operand(it);

          m_operand_list.emplace_back(std::move(lo));
          m_operand_list.emplace_back(std::move(ro));
        }

      else
        if((instr == std::string_view("ld8"))     ||
           (instr == std::string_view("ld16"))    ||
           (instr == std::string_view("ld32"))    ||
           (instr == std::string_view("ld64"))    ||
           (instr == std::string_view("log_not")) ||
           (instr == std::string_view("neg"))     ||
           (instr == std::string_view("bit_not")))
        {
          m_instruction = instr;

          auto  o = read_operand(it);

          m_operand_list.emplace_back(std::move(o));
        }

      else
        if(instr == std::string_view("phi"))
        {
          m_instruction = instr;

            if(it->is_operator_code("["))
            {
              m_operand_list.emplace_back(read_phi_element_list(++it));
            }

          else
            {
              throw ir_error("ir_operand error: have no phi_element");
            }
        }
    }

  else
    {
      throw ir_error("ir_operand error");
    }
}


void
ir_operation::
read(std::string_view  sv, token_iterator&  it)
{
  clear();

    if((sv == std::string_view("retv")) ||
       (sv == std::string_view("jmp")))
    {
      m_instruction = sv;

      auto  o = read_operand(it);

      m_operand_list.emplace_back(std::move(o));
    }

  else
    if((sv == std::string_view("nop"))||
       (sv == std::string_view("ret"))) 
    {
      m_instruction = sv;
    }

  else
    if(sv == std::string_view("br"))
    {
      m_instruction = sv;

      auto   cond_o = read_operand(it);
      auto   true_o = read_operand(it);
      auto  false_o = read_operand(it);

      m_operand_list.emplace_back(std::move( cond_o));
      m_operand_list.emplace_back(std::move( true_o));
      m_operand_list.emplace_back(std::move(false_o));
    }

  else
    if((sv == std::string_view("st8"))  ||
       (sv == std::string_view("st16")) ||
       (sv == std::string_view("st32")) ||
       (sv == std::string_view("st64")))
    {
      m_instruction = sv;

      auto  addr_o = read_operand(it);
      auto   val_o = read_operand(it);

      m_operand_list.emplace_back(std::move(addr_o));
      m_operand_list.emplace_back(std::move( val_o));
    }

  else
    {
      read_assign(sv,it);
    }


  set_kind(m_instruction);
}




}




