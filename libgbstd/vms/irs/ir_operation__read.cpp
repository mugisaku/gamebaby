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
/*

      else
        if(it->is_fpn())
        {
          return ir_operand(-it++->get_fpn());
        }
*/
    }
/*
  else
    if(it->is_fpn())
    {
      return ir_operand(it++->get_fpn());
    }
*/
  else
    if(it->is_identifier())
    {
      return ir_operand(it++->get_string());
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
}


void
ir_operation::
read(std::string_view  sv, token_iterator&  it)
{
  clear();

    if(sv == std::string_view("nop"))
    {
      m_operator_code = operator_code(sv);
    }

  else
    if(sv == std::string_view("ret"))
    {
      m_operator_code = operator_code(sv);

      auto  o = read_operand(it);

      m_operand_list.emplace_back(std::move(o));
    }

  else
    if(sv == std::string_view("br"))
    {
      m_operator_code = operator_code(sv);

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
      m_operator_code = operator_code(sv);

      auto  addr_o = read_operand(it);
      auto   val_o = read_operand(it);

      m_operand_list.emplace_back(std::move(addr_o));
      m_operand_list.emplace_back(std::move( val_o));
    }

  else
    if(it->is_identifier())
    {
      set_label(sv);

      auto&  opco = it++->get_string();

        if((opco == std::string_view("add")) ||
           (opco == std::string_view("sub")) ||
           (opco == std::string_view("mul")) ||
           (opco == std::string_view("div")) ||
           (opco == std::string_view("rem")) ||
           (opco == std::string_view("shl")) ||
           (opco == std::string_view("shr")) ||
           (opco == std::string_view("bit_or")) ||
           (opco == std::string_view("bit_and")) ||
           (opco == std::string_view("bit_xor")) ||
           (opco == std::string_view("eq")) ||
           (opco == std::string_view("neq")) ||
           (opco == std::string_view("lt")) ||
           (opco == std::string_view("lteq")) ||
           (opco == std::string_view("gt")) ||
           (opco == std::string_view("gteq")) ||
           (opco == std::string_view("log_or")) ||
           (opco == std::string_view("log_and")))
        {
          m_operator_code = operator_code(opco);

          auto  lo = read_operand(it);
          auto  ro = read_operand(it);

          m_operand_list.emplace_back(std::move(lo));
          m_operand_list.emplace_back(std::move(ro));
        }

      else
        if((opco == std::string_view("ld8"))     ||
           (opco == std::string_view("ld16"))    ||
           (opco == std::string_view("ld32"))    ||
           (opco == std::string_view("ld64"))    ||
           (opco == std::string_view("log_not")) ||
           (opco == std::string_view("neg"))     ||
           (opco == std::string_view("bit_not")))
        {
          m_operator_code = operator_code(opco);

          auto  o = read_operand(it);

          m_operand_list.emplace_back(std::move(o));
        }

      else
        if(opco == std::string_view("phi"))
        {
          m_operator_code = operator_code(opco);

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


  set_kind(m_operator_code);
}




}




