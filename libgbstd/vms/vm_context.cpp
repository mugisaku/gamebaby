#include"libgbstd/vm.hpp"




namespace gbstd{




namespace type_infos{
const type_info  null{};
const type_info  undefined{typesystem::undefined_type_info()};
const type_info  null_pointer{typesystem::null_pointer_type_info()};
const type_info         void_{typesystem::void_type_info()};
const type_info            s8{         typesystem::integer_type_info(1)};
const type_info            u8{typesystem::unsigned_integer_type_info(1)};
const type_info           s16{         typesystem::integer_type_info(2)};
const type_info           u16{typesystem::unsigned_integer_type_info(2)};
const type_info           s32{         typesystem::integer_type_info(4)};
const type_info           u32{typesystem::unsigned_integer_type_info(4)};
const type_info           s64{         typesystem::integer_type_info(8)};
const type_info           u64{typesystem::unsigned_integer_type_info(8)};
const type_info           f32{typesystem::fpn_type_info(4)};
const type_info           f64{typesystem::fpn_type_info(8)};
const type_info          boolean{typesystem::boolean_type_info(boolean_size)};
const type_info  generic_pointer{typesystem::generic_pointer_type_info(pointer_size)};
}




context::
context() noexcept:
m_memory(1024*1024*16)
{
  clear();
}


void
context::
clear() noexcept
{
  m_source.clear();
  m_type_collection.clear();
  m_type_info_table.clear();
  m_function_table.clear();
  m_global_symbol_table.clear();

  m_type_collection.push(  "int8_t",type_infos::s8);
  m_type_collection.push( "uint8_t",type_infos::u8);
  m_type_collection.push( "int16_t",type_infos::s16);
  m_type_collection.push("uint16_t",type_infos::u16);
  m_type_collection.push( "int32_t",type_infos::s32);
  m_type_collection.push("uint32_t",type_infos::u32);
  m_type_collection.push( "int64_t",type_infos::s64);
  m_type_collection.push("uint64_t",type_infos::u64);
  m_type_collection.push("float32_t",type_infos::f32);
  m_type_collection.push("float64_t",type_infos::f64);
  m_type_collection.push( "int",type_infos::s64);
  m_type_collection.push("uint",type_infos::u64);

  m_type_collection.push("bool",type_infos::boolean);

  m_type_collection.push("void",type_infos::void_);
  m_type_collection.push("nullptr_t",type_infos::null_pointer);

  m_type_collection.push("geneptr_t",type_infos::generic_pointer);

    while(m_number_of_frames)
    {
      pop_frame();
    }


  m_current_frame = nullptr;
}


const function&
context::
append_function(std::unique_ptr<function>&&  fn) noexcept
{
  m_function_table.emplace_back(std::move(fn));

  return *m_function_table.back();
}


const type_info&
context::
append_type_info(std::unique_ptr<type_info>&&  ti) noexcept
{
  m_type_info_table.emplace_back(std::move(ti));

  return *m_type_info_table.back();
}


void
context::
print() const noexcept
{
  printf("type collection{\n");

  m_type_collection.print();

  printf("}\ntype info table{\n");

    for(auto&  ti: m_type_info_table)
    {
      ti->print();

      printf("\n");
    }


  printf("}\nsymbol table{\n");

  m_global_symbol_table.print(m_memory);

  printf("}\nfunction table{\n");

    for(auto&  fn: m_function_table)
    {
      fn->print(*this);

      printf("\n");
    }


  printf("}\n");
}




context&
context::
assign(std::string_view  sv) noexcept
{
  clear();

  m_source = sv;

  token_block  blk(m_source);

  token_iterator  it(blk);

    while(it)
    {
        if(it->is_operator_code(";"))
        {
          ++it;
        }

      else
        {
            if(!read_declaration(it))
            {
              break;
            }
        }
    }


  return *this;
}


void
context::
reset() noexcept
{
  m_runtime_symbol_table = m_global_symbol_table;


}


void
context::
run() noexcept
{
    while(step())
    {
    }
}




context&
context::
finalize() noexcept
{
    for(auto&  sym: m_global_symbol_table)
    {
    }


  return *this;
}


}




