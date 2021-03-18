#ifndef gbstd_vm_ast_HPP
#define gbstd_vm_ast_HPP


#include"libgbstd/typesystem.hpp"
#include"libgbstd/misc.hpp"
#include"libgbstd/vms/expression.hpp"


namespace gbstd{




class
ast_error
{
  std::string  m_text;

  const token*  m_token=nullptr;

public:
  ast_error(std::string_view  sv) noexcept: m_text(sv){}
  ast_error(std::string_view  sv, const token&  tok) noexcept: m_text(sv), m_token(&tok){}

  void  print() const noexcept{  if(m_token){m_token->print();}  printf(": %s",m_text.data());}

};


class
ast_loopguard
{
  std::string_view  m_name;

  const token_iterator  m_iterator;

public:
  ast_loopguard(std::string_view  name, const token_iterator&  it) noexcept:
  m_name(name), m_iterator(it){}

  void  operator()(const token_iterator&  it) const{
      if(m_iterator == it)
      {
        auto  s = form_string("it is looping at %s",m_name.data());

        throw ast_error(s,*m_iterator);
      }
  }

};


class
ast_constant
{
  typesystem::type_info  m_type_info;

  std::vector<uint8_t>  m_memory;

public:
  ast_constant() noexcept{}
  ast_constant(bool  b) noexcept: m_type_info("bool8"), m_memory(1){get_i8() = b? 1:0;}
  ast_constant(int      i) noexcept: m_type_info("int64"), m_memory(8){get_i64() = i;}
  ast_constant(int64_t  i) noexcept: m_type_info("int64"), m_memory(8){get_i64() = i;}
  ast_constant(double  f) noexcept: m_type_info("float64"), m_memory(8){get_f64() = f;}
  ast_constant(std::string_view  sv) noexcept: m_type_info(typesystem::type_info("char8").form_array(sv.size())), m_memory(sv.size())
  {std::memcpy(m_memory.data(),sv.data(),sv.size());}

  operator bool() const noexcept{return m_type_info;}

  const typesystem::type_info*  operator->() const noexcept{return &m_type_info;}

  const typesystem::type_info&  get_type_info() const noexcept{return m_type_info;}

  int8_t&   get_i8(int  i=0)  noexcept{return reinterpret_cast< int8_t*>(m_memory.data())[i];}
  int16_t&  get_i16(int  i=0) noexcept{return reinterpret_cast<int16_t*>(m_memory.data())[i];}
  int32_t&  get_i32(int  i=0) noexcept{return reinterpret_cast<int32_t*>(m_memory.data())[i];}
  int64_t&  get_i64(int  i=0) noexcept{return reinterpret_cast<int64_t*>(m_memory.data())[i];}

  const int8_t&   get_i8(int  i=0)  const noexcept{return reinterpret_cast<const  int8_t*>(m_memory.data())[i];}
  const int16_t&  get_i16(int  i=0) const noexcept{return reinterpret_cast<const int16_t*>(m_memory.data())[i];}
  const int32_t&  get_i32(int  i=0) const noexcept{return reinterpret_cast<const int32_t*>(m_memory.data())[i];}
  const int64_t&  get_i64(int  i=0) const noexcept{return reinterpret_cast<const int64_t*>(m_memory.data())[i];}

  float&   get_f32(int  i=0) noexcept{return reinterpret_cast<float* >(m_memory.data())[i];}
  double&  get_f64(int  i=0) noexcept{return reinterpret_cast<double*>(m_memory.data())[i];}

  const float&   get_f32(int  i=0) const noexcept{return reinterpret_cast<const float* >(m_memory.data())[i];}
  const double&  get_f64(int  i=0) const noexcept{return reinterpret_cast<const double*>(m_memory.data())[i];}

};



namespace binary_operations{
ast_constant  add(const ast_constant&  l, const ast_constant&  r) noexcept;
ast_constant  sub(const ast_constant&  l, const ast_constant&  r) noexcept;
ast_constant  mul(const ast_constant&  l, const ast_constant&  r) noexcept;
ast_constant  div(const ast_constant&  l, const ast_constant&  r) noexcept;
}


class
ast_content
{
  template<class  T>
  struct default_deleter{
    using fnptr_t = void(*)(void*);

    static void  process(T*  ptr) noexcept{delete ptr;}
    static fnptr_t  get() noexcept{return reinterpret_cast<fnptr_t>(&process);}
  };

  const token*  m_token=nullptr;

  void*  m_data=nullptr;

  void  (*m_deleter)(void*)=nullptr;

public:
  ast_content() noexcept{}
  ast_content(const token&  tok) noexcept: m_token(&tok){}
 ~ast_content(){if(m_deleter){m_deleter(m_data);}}

  ast_content(const ast_content& ) noexcept=delete;
  ast_content(      ast_content&&  rhs) noexcept{assign(std::move(rhs));}

  ast_content&  operator=(const ast_content& ) noexcept=delete;
  ast_content&  operator=(      ast_content&&  rhs) noexcept{return assign(std::move(rhs));}

  const token&  operator*()  const noexcept{return *m_token;}
  const token*  operator->() const noexcept{return  m_token;}

  ast_content&  assign(ast_content&&  rhs) noexcept;

  void          set_token(const token*  tok)       noexcept{       m_token = tok;}
  const token*  get_token(                 ) const noexcept{return m_token;}

  template<class  T, class  DELETER=default_deleter<T>>
  void  set_data(T*  ptr) noexcept{
      if(m_deleter)
      {
        m_deleter(m_data);
      }


    m_data    =            ptr;
    m_deleter = DELETER::get();
  }

  template<class  T>
  pointer_wrapper<T>  get_data() const noexcept{return static_cast<T*>(m_data);}

};


using ast_node = basic_node<ast_content>;

inline ast_node::child_type
make_ast_node(std::string_view  name) noexcept
{
  return std::make_unique<ast_node>(name);
}

inline ast_node::child_type
make_ast_node(const token&  tok, std::string_view  name) noexcept
{
  auto  nd = std::make_unique<ast_node>(name);

  nd->content().set_token(&tok);

  return std::move(nd);
}

inline ast_node::child_type
rename(ast_node::child_type&&  child, std::string_view  name) noexcept
{
  child->set_name(name);

  return std::move(child);
}


void  print(const ast_node&  nd) noexcept;




std::unique_ptr<ast_node>  read_global_space_element(token_iterator&  it, const ast_node&  base_nd);


class
ast_builder
{
  std::vector<const ast_node*>  m_routes;

  token_iterator  m_iterator;

  std::unique_ptr<ast_node>  read_argument_list();
  std::unique_ptr<ast_node>  read_index();
  std::unique_ptr<ast_node>  read_operand();
  std::unique_ptr<ast_node>  read_primary_expression_element();
  std::unique_ptr<ast_node>  read_primary_expression();
  std::unique_ptr<ast_node>  read_unary_operator();
  std::unique_ptr<ast_node>  read_unary_expression();
  std::unique_ptr<ast_node>  read_binary_operator();
  std::unique_ptr<ast_node>  read_expression_element();
  std::unique_ptr<ast_node>  read_expression();

  std::unique_ptr<ast_node>  read_let( );
  std::unique_ptr<ast_node>  read_return();
  std::unique_ptr<ast_node>  read_goto();
  std::unique_ptr<ast_node>  read_case();
  std::unique_ptr<ast_node>  read_switch();
  std::unique_ptr<ast_node>  read_for_instruction();
  std::unique_ptr<ast_node>  read_for();
  std::unique_ptr<ast_node>  read_while();
  std::unique_ptr<ast_node>  read_if();
  std::unique_ptr<ast_node>  read_if_string();
  std::unique_ptr<ast_node>  read_statement();

  std::unique_ptr<ast_node>  read_parameter();
  std::unique_ptr<ast_node>  read_parameter_list();
  std::unique_ptr<ast_node>  read_local_space();
  std::unique_ptr<ast_node>  read_function();
  std::unique_ptr<ast_node>  read_struct_def();
  std::unique_ptr<ast_node>  read_struct();
  std::unique_ptr<ast_node>  read_union_def();
  std::unique_ptr<ast_node>  read_union();
  std::unique_ptr<ast_node>  read_enumerator();
  std::unique_ptr<ast_node>  read_enum_def();
  std::unique_ptr<ast_node>  read_enum();
  std::unique_ptr<ast_node>  read_alias();
  std::unique_ptr<ast_node>  read_type();
  std::unique_ptr<ast_node>  read_name();

  std::unique_ptr<ast_node>  read_global_space_element();

  void  advance(int  n=1) noexcept{m_iterator += n;}

public:
  ast_builder(token_iterator  it) noexcept: m_iterator(it){}

  void  push(const std::unique_ptr<ast_node>&  nd) noexcept{m_routes.emplace_back(nd.get());}
  void   pop() noexcept{m_routes.pop_back();}

  const ast_node*  get_base() const noexcept{return m_routes.empty()? nullptr:m_routes.front();}

  const ast_node*  find_node(std::string_view  name) const noexcept;

  typesystem::type_info  find_type_info(std::string_view  name) const noexcept;

  const ast_constant*  find_constant(std::string_view  name) const noexcept;

  std::unique_ptr<ast_node>  read_global_space();

  void  print() const noexcept;

};


}




#endif




