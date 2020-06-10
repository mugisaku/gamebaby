#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


void
global_space::
initialize() noexcept
{
  m_type_info_table.emplace_back(integer_type_info(4),"int");

  m_type_info_table.emplace_back(integer_type_info(1),"int8_t");
  m_type_info_table.emplace_back(integer_type_info(2),"int16_t");
  m_type_info_table.emplace_back(integer_type_info(4),"int32_t");
  m_type_info_table.emplace_back(integer_type_info(8),"int64_t");
  m_type_info_table.emplace_back(fpn_type_info(4),"f32_t");
  m_type_info_table.emplace_back(fpn_type_info(8),"f64_t");
  m_type_info_table.emplace_back(null_pointer_type_info(),"nullptr_t");
  m_type_info_table.emplace_back(boolean_type_info(1),"bool");
  m_type_info_table.emplace_back(void_type_info(),"void");
}


global_space&
global_space::
assign(std::string_view  sv)
{
  clear();

  initialize();

  auto  toks = make_token_string(sv);

  token_iterator  it(toks);

  read(it,"");

  return *this;
}




void
global_space::
compile(compile_context&  ctx) const
{
  std::vector<std::reference_wrapper<function>>           fn_list;
  std::vector<std::reference_wrapper<variable_info>>  glovar_list;

  m_node.collect_functions(fn_list);
  m_node.collect_global_variables(glovar_list);

    for(auto&  glovarref: glovar_list)
    {
    }


    for(auto&  st: m_statement_list)
    {
      st.compile(ctx);

      ctx.m_block_number++;
    }


    for(auto&  fnref: fn_list)
    {
      char  buf[256];

      snprintf(buf,sizeof(buf),"FN_%04d",ctx.m_block_number);

      ctx.m_block_number++;

      fnref.get().compile(ctx);
    }
}


void
global_space::
print() const noexcept
{
  basic_space::print();
}




}




