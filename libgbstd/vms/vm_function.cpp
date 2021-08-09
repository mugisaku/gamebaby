#include"libgbstd/vm.hpp"




namespace gbstd{




vm_operation
vm_function::
construct_unary_operation(const syntax_branch&  br) noexcept
{
  auto  op = construct_unary_opcode(br[0].branch());

  auto  l = construct_operand(br[1].branch());

  return vm_operation(op,std::move(l));
}


vm_operation
vm_function::
construct_binary_operation(const syntax_branch&  br) noexcept
{
  auto  op = construct_binary_opcode(br[0].branch());

  auto  l = construct_operand(br[1].branch());
  auto  r = construct_operand(br[2].branch());

  return vm_operation(op,std::move(l),std::move(r));
}


vm_opcode
vm_function::
construct_unary_opcode(const syntax_branch&  br) noexcept
{
  auto  s = br[0].string();

  return (s == u"logi_not")? vm_opcode::logi_not
        :(s == u"not")? vm_opcode::not_
        :(s == u"neg")? vm_opcode::neg
        :(s == u"fneg")? vm_opcode::fneg
        :vm_opcode::nop
        ;
}


vm_opcode
vm_function::
construct_binary_opcode(const syntax_branch&  br) noexcept
{
  auto  s = br[0].string();

  return (s == u"add")? vm_opcode::add
        :(s == u"sub")? vm_opcode::sub
        :(s == u"mul")? vm_opcode::mul
        :(s == u"div")? vm_opcode::div
        :(s == u"rem")? vm_opcode::rem
        :(s == u"fadd")? vm_opcode::fadd
        :(s == u"fsub")? vm_opcode::fsub
        :(s == u"fmul")? vm_opcode::fmul
        :(s == u"fdiv")? vm_opcode::fdiv
        :(s == u"frem")? vm_opcode::frem
        :(s == u"eq")? vm_opcode::eq
        :(s == u"neq")? vm_opcode::neq
        :(s == u"lt")? vm_opcode::lt
        :(s == u"lteq")? vm_opcode::lteq
        :(s == u"gt")? vm_opcode::gt
        :(s == u"gteq")? vm_opcode::gteq
        :(s == u"feq")? vm_opcode::feq
        :(s == u"fneq")? vm_opcode::fneq
        :(s == u"flt")? vm_opcode::flt
        :(s == u"flteq")? vm_opcode::flteq
        :(s == u"fgt")? vm_opcode::fgt
        :(s == u"fgteq")? vm_opcode::fgteq
        :(s == u"shl")? vm_opcode::shl
        :(s == u"shr")? vm_opcode::shr
        :(s == u"and")? vm_opcode::and_
        :(s == u"or")? vm_opcode::or_
        :(s == u"xor")? vm_opcode::xor_
        :(s == u"logi_and")? vm_opcode::logi_and
        :(s == u"logi_or")? vm_opcode::logi_or
        :vm_opcode::nop
        ;
}


void
vm_function::
construct_address_operation(const syntax_branch&  br) noexcept
{
}


vm_phi_element
vm_function::
construct_phi_element(const syntax_branch&  br) noexcept
{
  return vm_phi_element(br[0].string(),construct_operand(br[1].branch()));
}


vm_phi_element_list
vm_function::
construct_phi_operation(const syntax_branch&  br) noexcept
{
  vm_phi_element_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"phi_element"))
        {
          ls.emplace_back(construct_phi_element(e.branch()));
        }
    }


  return std::move(ls);
}


vm_register
vm_function::
construct_operations(const syntax_branch&  br, vm_type_info  ti, std::u16string_view  lb) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"unary_operation")  ){return vm_register(ti,lb,construct_unary_operation(  e.branch()));}
  else if(e.is_branch(u"binary_operation") ){return vm_register(ti,lb,construct_binary_operation( e.branch()));}
//  else if(e.is_branch(u"address_operation")){construct_address_operation(e.branch());}
  else if(e.is_branch(u"phi_operation")    ){return vm_register(ti,lb,construct_phi_operation(e.branch()));}
  else{}

  return vm_register();
}


vm_register
vm_function::
construct_register(const syntax_branch&  br) noexcept
{
  auto  ti = construct_type_info(br[1].branch());

  auto  lb = br[2].string();

  auto&  e = br[3];

       if(e.is_branch(u"operation")     ){return construct_operations(e.branch(),ti,lb);}
  else if(e.is_branch(u"call_statement")){return vm_register(ti,lb,construct_call(e.branch()));}

  return vm_register();
}


vm_return
vm_function::
construct_return(const syntax_branch&  br) noexcept
{
    if(br.length() == 2)
    {
      return vm_return(construct_operand(br[1].branch()));
    }


  return vm_return();
}


vm_branch
vm_function::
construct_branch(const syntax_branch&  br) noexcept
{
  return vm_branch(construct_operand(br[0].branch()),br[1].string(),br[2].string());
}


void
vm_function::
construct_label_statement(const syntax_branch&  br) noexcept
{
  add_block(br[1].string());
}


vm_transfer
vm_function::
construct_store(const syntax_branch&  br) noexcept
{
//  construct_type_info(br[1].branch());

//  br[2].string();
//  br[3].string();

  return vm_transfer();
}


vm_operand_list
vm_function::
construct_operand_list(const syntax_branch&  br) noexcept
{
  vm_operand_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"operand"))
        {
          ls.emplace_back(construct_operand(e.branch()));
        }
    }


  return std::move(ls);
}


vm_call
vm_function::
construct_call(const syntax_branch&  br) noexcept
{
  return vm_call(br[1].string(),construct_operand_list(br[3].branch()));
}


void
vm_function::
construct_statement(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"register_statement")){add_line(vm_line(construct_register(e.branch())));}
  else if(e.is_branch(u"return_statement")  ){add_line(vm_line(construct_return(e.branch())));}
  else if(e.is_branch(u"branch_statement")  ){add_line(vm_line(construct_branch(e.branch())));}
  else if(e.is_branch(u"label_statement")   ){construct_label_statement(e.branch());}
  else if(e.is_branch(u"store_statement")   ){add_line(vm_line(construct_store(e.branch())));}
  else if(e.is_branch(u"call_statement")    ){add_line(vm_line(construct_call(e.branch())));}
  else{;}
}


vm_parameter
vm_function::
construct_parameter(const syntax_branch&  br) noexcept
{
  auto  ti = construct_type_info(br[0].branch());

  return vm_parameter(ti,br[1].string());
}


vm_parameter_list
vm_function::
construct_parameter_list(const syntax_branch&  br) noexcept
{
  vm_parameter_list  ls;

    for(auto&  e: br)
    {
        if(e.is_branch(u"parameter"))
        {
          ls.emplace_back(construct_parameter(e.branch()));
        }
    }


  return std::move(ls);
}


vm_type_info
vm_function::
construct_type_info(const syntax_branch&  br) noexcept
{
  auto  s = br[0].string();

  return (s == u"i8")?  vm_type_info('i',1)
        :(s == u"i16")? vm_type_info('i',2)
        :(s == u"i32")? vm_type_info('i',4)
        :(s == u"i64")? vm_type_info('i',8)
        :vm_type_info()
        ;
}


vm_operand
vm_function::
construct_operand(const syntax_branch&  br) noexcept
{
  auto&  tok = br[0].token();

  return tok.is_integer()?    vm_operand(tok.integer())
        :tok.is_floating()?   vm_operand(tok.floating())
        :tok.is_identifier()? vm_operand(tok.string())
        :vm_operand()
        ;
}


void
vm_function::
construct_statement_list(const syntax_branch&  br) noexcept
{
    for(auto&  e: br)
    {
        if(e.is_branch(u"statement"))
        {
          construct_statement(e.branch());
        }
    }
}


vm_function&
vm_function::
assign(const syntax_branch&  br) noexcept
{
  m_block_array.clear();

  m_symbol_table.clear();

  m_type_info = construct_type_info(br[1].branch());

  auto  parals = construct_parameter_list(br[3].branch());

  m_argument_size         = 0;
  m_stack_allocation_size = 0;

    for(auto&  para: parals)
    {
      auto&  sym = m_symbol_table.emplace_back(para.type_info(),para.name(),'l');

      sym.set_offset(m_argument_size);

      m_argument_size += para.type_info().max_size();
    }

    
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__previous_bp__",'l').set_offset(m_argument_size);
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__current_sp__",'l').set_offset(m_argument_size+8);
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__return_address__",'l').set_offset(m_argument_size+16);
  m_symbol_table.emplace_back(vm_type_info('i',8),u"__return_value_address__",'l').set_offset(m_argument_size+24);

  m_stack_allocation_size = m_argument_size+32;

  m_name = br[5].string();

  m_label = std::u16string(u"__FN__")+std::u16string(m_name)+std::u16string(u"__START__");

  add_block(m_label);

  construct_statement_list(br[7].branch());

  return *this;
}




vm_line&
vm_function::
add_line(vm_line&&  ln) noexcept
{
    if(m_block_array.empty())
    {
      add_block(u"");
    }


  auto&  blk = m_block_array.back();

  return blk.lines().emplace_back(std::move(ln));
}


vm_block&
vm_function::
add_block(std::u16string_view  lb) noexcept
{
  return m_block_array.emplace_back(lb);
}


const vm_block*
vm_function::
find_block(std::u16string_view  label) const noexcept
{
    for(auto&  blk: m_block_array)
    {
        if(blk.label() == label)
        {
          return &blk;
        }
    }


  return nullptr;
}


int
vm_function::
finalize(int  pos) noexcept
{
    for(auto&  blk: m_block_array)
    {
        for(auto&  ln: blk.lines())
        {
          ln.set_position(pos++);

            if(ln.is_register())
            {
              auto&  reg = ln.get_register();

              m_symbol_table.emplace_back(reg.type_info(),reg.label(),'l').set_offset(m_stack_allocation_size);

              m_stack_allocation_size += reg.type_info().max_size();
            }
        }
    }


    for(auto&  sym: m_symbol_table)
    {
      sym.add_offset(-m_argument_size);
    }


  return pos;
}


void
vm_function::
print() const noexcept
{
  printf("function ");

  m_type_info.print();

  printf(" ");

  gbstd::print(m_name);

  printf("\n");

    for(auto&  sym: m_symbol_table)
    {
      sym.print();

      printf("\n");
    }


  printf("\n");

    for(auto&  blk: m_block_array)
    {
      gbstd::print(blk.label());

      printf(":\n");

        for(auto&  ln: blk.lines())
        {
          ln.print();

          printf("\n");
        }
    }
}




}




