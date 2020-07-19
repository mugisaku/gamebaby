#include"libgbstd/vm.hpp"




namespace gbstd{


using namespace typesystem;


void
global_space::
initialize() noexcept
{
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
      st.compile(m_node,ctx);
    }


    for(auto&  fnref: fn_list)
    {
//      fnref.get().compile(ctx);
    }
}


void
global_space::
print() const noexcept
{
  basic_space::print();
}




}




