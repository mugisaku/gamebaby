#include"libgbstd/vm.hpp"




namespace gbstd{




void
vm_function::
construct_define_operation(const syntax_branch&  br, vm_line&  ln) noexcept
{
  ln.set_opcode(vm_opcode::ld);

  auto&  tok = br[1].token();

    if(tok.is_integer())
    {
      ln.add_operand(tok.integer());
    }

  else
    if(tok.is_floating())
    {
      ln.add_operand(tok.floating());
    }
}


void
vm_function::
construct_unary_operation(const syntax_branch&  br, vm_line&  ln) noexcept
{
}


void
vm_function::
construct_binary_instruction(const syntax_branch&  br, vm_line&  ln) noexcept
{
  auto  s = br[0].string();

  ln.set_opcode( (s == u"add")? vm_opcode::add
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
               )
               ;
}


void
vm_function::
construct_binary_operation(const syntax_branch&  br, vm_line&  ln) noexcept
{
  construct_binary_instruction(br[0].branch(),ln);

  ln.add_operand(construct_operand(br[1].branch()));
  ln.add_operand(construct_operand(br[2].branch()));
}


void
vm_function::
construct_address_operation(const syntax_branch&  br, vm_line&  ln) noexcept
{
}


void
vm_function::
construct_phi_operation(const syntax_branch&  br, vm_line&  ln) noexcept
{
}


void
vm_function::
construct_operation(const syntax_branch&  br, vm_line&  ln) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"define_operation") ){construct_define_operation( e.branch(),ln);}
  else if(e.is_branch(u"unary_operation")  ){construct_unary_operation(  e.branch(),ln);}
  else if(e.is_branch(u"binary_operation") ){construct_binary_operation( e.branch(),ln);}
  else if(e.is_branch(u"address_operation")){construct_address_operation(e.branch(),ln);}
  else if(e.is_branch(u"phi_operation")    ){construct_phi_operation(    e.branch(),ln);}
  else{}
}


void
vm_function::
construct_register_statement(const syntax_branch&  br, vm_line&  ln) noexcept
{
  ln.set_type_info(construct_type_info(br[1].branch()));

  ln.set_destination(br[2].string());


  auto&  e = br[3];

       if(e.is_branch(u"operation")     ){construct_operation(e.branch(),ln);}
  else if(e.is_branch(u"call_statement")){construct_call_statement(e.branch(),ln);}
}


void
vm_function::
construct_return_statement(const syntax_branch&  br, vm_line&  ln) noexcept
{
  ln.set_opcode(vm_opcode::ret);

    if(br.length() == 2)
    {
      ln.add_operand(construct_operand(br[1].branch()));
    }
}


void
vm_function::
construct_branch_statement(const syntax_branch&  br, vm_line&  ln) noexcept
{
  gbstd::print(br[1].string());
  gbstd::print(br[2].string());
}


void
vm_function::
construct_label_statement(const syntax_branch&  br) noexcept
{
  add_block(br[1].string());
}


void
vm_function::
construct_store_statement(const syntax_branch&  br, vm_line&  ln) noexcept
{
//  construct_type_info(br[1].branch());

  gbstd::print(br[2].string());
  gbstd::print(br[3].string());
}


void
vm_function::
construct_argument_list(const syntax_branch&  br, vm_line&  ln) noexcept
{
    for(auto&  e: br)
    {
        if(e.is_branch(u"operand"))
        {
          ln.add_operand(construct_operand(e.branch()));
        }
    }
}


void
vm_function::
construct_call_statement(const syntax_branch&  br, vm_line&  ln) noexcept
{
  ln.set_opcode(vm_opcode::cal);

  ln.add_operand(br[1].string());

  construct_argument_list(br[3].branch(),ln);
}


void
vm_function::
construct_statement(const syntax_branch&  br) noexcept
{
  auto&  e = br[0];

       if(e.is_branch(u"register_statement")){construct_register_statement(e.branch(),add_line());}
  else if(e.is_branch(u"return_statement")  ){construct_return_statement(e.branch(),add_line());}
  else if(e.is_branch(u"branch_statement")  ){construct_branch_statement(e.branch(),add_line());}
  else if(e.is_branch(u"label_statement")   ){construct_label_statement(e.branch());}
  else if(e.is_branch(u"store_statement")   ){construct_store_statement(e.branch(),add_line());}
  else if(e.is_branch(u"call_statement")    ){construct_call_statement(e.branch(),add_line());}
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

  m_stack_allocation_size = 32;

  m_name = br[5].string();

  m_label = std::u16string(u"__FN__")+std::u16string(m_name)+std::u16string(u"__START__");

  add_block(m_label);

  construct_statement_list(br[7].branch());

  return *this;
}




vm_line&
vm_function::
add_line() noexcept
{
    if(m_block_array.empty())
    {
      add_block(u"");
    }


  auto&  blk = m_block_array.back();

  return blk.lines().emplace_back();
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

            if(ln.has_destination())
            {
              m_symbol_table.emplace_back(ln.type_info(),ln.destination(),'l').set_offset(m_stack_allocation_size);

              m_stack_allocation_size += ln.type_info().max_size();
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




