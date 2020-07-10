#ifndef gbstd_vm_ir_HPP
#define gbstd_vm_ir_HPP


#include"libgbstd/parser.hpp"
#include"libgbstd/vms/expression.hpp"




namespace gbstd{




class ir_operand;
class ir_operation;
class ir_phi_element;


class
ir_error
{
public:
  std::string  m_comment;

  ir_error(std::string_view  sv) noexcept{m_comment = sv;}

};




class
ir_address
{
  uint32_t  m_index=0;
  uint32_t  m_offset=0;

public:
  constexpr ir_address() noexcept{}
  constexpr ir_address(uint32_t  i, uint32_t  off) noexcept: m_index(i), m_offset(off){}
  constexpr ir_address(int64_t  i) noexcept: m_index(i>>32), m_offset(i&0xFFFFFFFF){}

  constexpr operator bool() const noexcept{return m_index;}

  constexpr uint32_t  get_index()  const noexcept{return m_index;}
  constexpr uint32_t  get_offset() const noexcept{return m_offset;}

  constexpr int64_t  get_word() const noexcept{return (static_cast<int64_t>(m_index)<<32)|m_offset;}

};




struct ir_undefined{};


class
ir_type_info
{
  enum class kinds{
    null, undefined, void_, integer, fpn, object,
  } m_kind=kinds::null;

  int  m_size=0;

  static constexpr bool  is_obj(std::string_view  sv) noexcept{
    return((sv.size() > 3) &&
           (sv[0] == 'o')  &&
           (sv[1] == 'b')  &&
           (sv[2] == 'j'));
  }

  static constexpr int  to_i(char  c) noexcept{
    return ((c >= '0') && (c <= '9'))? c-'0':0;
  }

  static constexpr int  get_i(const char*  s, int  i) noexcept{
    return (s[0] == 0)? i: get_i(s+1,(i*10)+to_i(*s));
  }

  static constexpr kinds  get_kind(std::string_view  sv) noexcept{
    return (sv == std::string_view("void"))?      kinds::void_
          :(sv == std::string_view("undefined"))? kinds::undefined
          :(sv == std::string_view("int"))?       kinds::integer
          :(sv == std::string_view("float"))?     kinds::fpn
          :is_obj(sv)?                            kinds::object
          :kinds::null
          ;
  }

  static constexpr int  get_size(std::string_view  sv) noexcept{
    return (sv == std::string_view("void")     )? 0
          :(sv == std::string_view("undefined"))? 0
          :(sv == std::string_view("int")      )? sizeof(int64_t)
          :(sv == std::string_view("float")    )? sizeof(int64_t)
          :is_obj(sv)?                            get_i(sv.data()+3,0)
          :0
          ;
  }

public:
  constexpr ir_type_info() noexcept{}
  constexpr ir_type_info(int  sz) noexcept: m_kind(kinds::object), m_size(sz){}
  constexpr ir_type_info(std::string_view  sv) noexcept: m_kind(get_kind(sv)), m_size(get_size(sv)){}

  constexpr bool  is_void()      const noexcept{return m_kind == kinds::void_;}
  constexpr bool  is_undefined() const noexcept{return m_kind == kinds::undefined;}
  constexpr bool  is_integer()   const noexcept{return m_kind == kinds::integer;}
  constexpr bool  is_fpn()       const noexcept{return m_kind == kinds::fpn;}
  constexpr bool  is_object()    const noexcept{return m_kind == kinds::object;}

  constexpr int  get_size()  const noexcept{return m_size;}

  void  print() const noexcept{
      switch(m_kind)
      {
    case(kinds::null     ): printf("null");break;
    case(kinds::void_    ): printf("void");break;
    case(kinds::undefined): printf("undefined");break;
    case(kinds::integer  ): printf("int");break;
    case(kinds::fpn      ): printf("float");break;
    case(kinds::object   ): printf("obj%d",m_size);break;
      }
  }

};


class
ir_memory
{
  int64_t*  m_data=nullptr;

  void  unrefer() noexcept;

  static uint8_t  m_dummy;

  uint8_t*  get_head_pointer() const noexcept;

public:
  ir_memory() noexcept{}
  ir_memory(const ir_memory&   rhs) noexcept{assign(rhs);}
  ir_memory(      ir_memory&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_memory(){unrefer();}

  template<class...  Args>
  ir_memory(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  ir_memory&  operator=(const ir_memory&   rhs) noexcept{return assign(rhs);}
  ir_memory&  operator=(      ir_memory&&  rhs) noexcept{return assign(std::move(rhs));}

  template<class...  Args>
  ir_memory&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  uint8_t&  operator[](int  i) const noexcept{return get_head_pointer()[i];}

  operator bool() const noexcept{return m_data;}

  ir_memory&  assign(const ir_memory&   rhs) noexcept;
  ir_memory&  assign(      ir_memory&&  rhs) noexcept;
  ir_memory&  assign(ir_type_info  ti) noexcept;
  ir_memory&  assign(int64_t  i) noexcept;
  ir_memory&  assign(double  f) noexcept;
  ir_memory&  assign(std::string_view  sv) noexcept;

  template<class  T>
  T&  get_content(int  offset=0) const noexcept{return reinterpret_cast<T&>(get_head_pointer()[offset]);}

  std::string_view  get_string() const noexcept{return std::string_view(&get_content<char>(),get_size());}

  int64_t  get_size() const noexcept{return m_data? m_data[1]:0;}

  ir_memory  clone() const noexcept{return ir_memory(get_string());}

};




class
ir_value
{
protected:
  ir_type_info  m_type_info;

  ir_memory  m_memory;

public:
   ir_value() noexcept{}
   ir_value(const ir_value&   rhs) noexcept{assign(rhs);}
   ir_value(      ir_value&&  rhs) noexcept{assign(std::move(rhs));}

   template<class...  Args>
   ir_value(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

   ir_value&  operator=(const ir_value&   rhs) noexcept{return assign(rhs);}
   ir_value&  operator=(      ir_value&&  rhs) noexcept{return assign(std::move(rhs));}

   template<class...  Args>
   ir_value&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

   ir_value&  assign(const ir_value&   rhs) noexcept;
   ir_value&  assign(      ir_value&&  rhs) noexcept;
   ir_value&  assign(ir_type_info  ti) noexcept;
   ir_value&  assign(ir_type_info  ti, ir_memory&&  mem) noexcept;
   ir_value&  assign(bool  b) noexcept{return assign(static_cast<int64_t>(b? 1:0));}
   ir_value&  assign(int64_t  i) noexcept;
   ir_value&  assign(double  f) noexcept;
   ir_value&  assign(std::string_view  sv) noexcept;

  void  clear() noexcept;

  const ir_type_info*  operator->() const noexcept{return &m_type_info;}

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}
  const ir_memory&     get_memory()    const noexcept{return m_memory;}

  ir_memory  release_memory() noexcept{  m_type_info = ir_type_info();  return std::move(m_memory);}

  int64_t           get_integer() const noexcept{return m_memory.get_content<int64_t>();}
  double            get_fpn()     const noexcept{return m_memory.get_content<double>();}
  std::string_view  get_string()  const noexcept{return m_memory.get_string();}

  ir_value  clone() const noexcept;

  void  print() const noexcept;

};


class
ir_operand
{
  enum class kinds{
    null, integer, fpn, object, label, phi_element_list, operand_list
  } m_kind=kinds::null;

  union data{
    int64_t  i;
    double   f;

    std::string  s;

    std::vector<ir_phi_element>  phels;
    std::vector<ir_operand>       opls;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  ir_operand() noexcept{}
  ir_operand(const ir_operand&   rhs) noexcept{assign(rhs);}
  ir_operand(      ir_operand&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_operand(){clear();}

  template<class...  Args>
  explicit ir_operand(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  ir_operand&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  ir_operand&  operator=(const ir_operand&   rhs) noexcept{return assign(rhs);}
  ir_operand&  operator=(      ir_operand&&  rhs) noexcept{return assign(std::move(rhs));}

  ir_operand&  assign(const ir_operand&   rhs) noexcept;
  ir_operand&  assign(      ir_operand&&  rhs) noexcept;
  ir_operand&  assign(int      i) noexcept{return assign(static_cast<int64_t>(i));}
  ir_operand&  assign(int64_t  i) noexcept;
  ir_operand&  assign(double   f) noexcept;
  ir_operand&  assign(const void*  ptr, size_t  n) noexcept;
  ir_operand&  assign(std::string_view  sv) noexcept;
  ir_operand&  assign(std::vector<ir_phi_element>&&  phels) noexcept;
  ir_operand&  assign(std::vector<ir_operand>&&  opls) noexcept;

  void  clear() noexcept;

  bool  is_integer()          const noexcept{return m_kind == kinds::integer;}
  bool  is_fpn()              const noexcept{return m_kind == kinds::fpn;}
  bool  is_object()           const noexcept{return m_kind == kinds::object;}
  bool  is_label()            const noexcept{return m_kind == kinds::label;}
  bool  is_phi_element_list() const noexcept{return m_kind == kinds::phi_element_list;}
  bool  is_operand_list()     const noexcept{return m_kind == kinds::operand_list;}

  int64_t                             get_integer()          const noexcept{return m_data.i;}
  double                              get_fpn()              const noexcept{return m_data.f;}
  const std::string&                  get_string()           const noexcept{return m_data.s;}
  const std::vector<ir_phi_element>&  get_phi_element_list() const noexcept{return m_data.phels;}
  const std::vector<ir_operand>&      get_operand_list()     const noexcept{return m_data.opls;}

  void  print() const noexcept;

};


class
ir_phi_element
{
  std::string  m_label;

  ir_operand  m_operand;

public:
  template<class  T>
  ir_phi_element(std::string_view  lb, T&&  t) noexcept:
  m_label(lb), m_operand(std::move(t)){}

  const std::string&  get_label() const noexcept{return m_label;}

  const ir_operand&  get_operand() const noexcept{return m_operand;}

  void  print() const noexcept{
    printf("%s:",m_label.data());
    m_operand.print();
  }

};




class
ir_block_info
{
  std::string  m_label;

  const ir_operation*  m_entry=nullptr;

  int  m_number_of_operations=0;

public:
  ir_block_info() noexcept{}
  ir_block_info(std::string_view  lb) noexcept: m_label(lb){}

  ir_block_info&  operator++() noexcept;

  void                 set_entry(const ir_operation*  e)       noexcept{m_entry = e;}
  const ir_operation*  get_entry(                      ) const noexcept{return m_entry;}

  const ir_operation*  get_end() const noexcept;

  int  get_number_of_operations() const noexcept{return m_number_of_operations;}

  void  set_label(std::string_view  sv) noexcept{m_label = sv;}

  const std::string&  get_label() const noexcept{return m_label;}

  bool  test_label(std::string_view  label) const noexcept{return m_label == label;}

  void  print() const noexcept;

};


class
ir_operation
{
  const ir_block_info*  m_block_info=nullptr;

  ir_type_info  m_type_info=ir_type_info("undefined");

  char*  m_label=nullptr;

  int  m_label_length=0;

  std::vector<ir_operand>  m_operand_list;

  std::string  m_instruction;

  enum class kinds{
    arithmetic,
    bitwise,
    comparison,
    load,
    store,
    others,

  } m_kind;


  void  set_kind(std::string_view  sv) noexcept;

  void  read_void(token_iterator&  it);
  void  read_non_void(token_iterator&  it);

public:
  ir_operation() noexcept{}
  ir_operation(ir_type_info  ti) noexcept: m_type_info(ti){}
  ir_operation(const ir_block_info&  bi, ir_type_info  ti, std::string_view  lb, std::string_view  instr, std::vector<ir_operand>&&  opls={}) noexcept;

  ir_operation(const ir_operation&   rhs) noexcept{assign(rhs);}
  ir_operation(      ir_operation&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_operation(){clear();}

  ir_operation&  operator=(const ir_operation&   rhs) noexcept{return assign(rhs);}
  ir_operation&  operator=(      ir_operation&&  rhs) noexcept{return assign(std::move(rhs));}

  ir_operation&  assign(const ir_operation&   rhs) noexcept;
  ir_operation&  assign(      ir_operation&&  rhs) noexcept;

  void  clear() noexcept;

  bool  is_arithmetic() const noexcept{return m_kind == kinds::arithmetic;}
  bool  is_bitwise()    const noexcept{return m_kind == kinds::bitwise;}
  bool  is_comparison() const noexcept{return m_kind == kinds::comparison;}
  bool  is_load()       const noexcept{return m_kind == kinds::load;}
  bool  is_store()      const noexcept{return m_kind == kinds::store;}
  bool  is_others()     const noexcept{return m_kind == kinds::others;}

  void  read(ir_type_info  ti, token_iterator&  it);

  ir_operation&     set_label(std::string_view  sv) noexcept;
  std::string_view  get_label() const noexcept{return std::string_view(m_label,m_label_length);}

  ir_operation&         set_block_info(const ir_block_info*  bi)       noexcept{  m_block_info = bi;  return *this;}
  const ir_block_info*  get_block_info(                        ) const noexcept{return m_block_info;}

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::vector<ir_operand>&  get_operand_list() const noexcept{return m_operand_list;}

  void                set_instruction(std::string_view  sv) noexcept{  m_instruction = sv;  set_kind(sv);}
  const std::string&  get_instruction(                    ) const noexcept{return m_instruction;}

  void  print() const noexcept;

};


class
ir_parameter
{
  ir_type_info  m_type_info;

  std::string  m_label;

public:
  ir_parameter() noexcept{}
  ir_parameter(ir_type_info  ti, std::string_view  lb) noexcept: m_type_info(ti), m_label(lb){}

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_label() const noexcept{return m_label;}

};


class
ir_function
{
  ir_type_info  m_type_info;

  std::string  m_name;

  std::vector<ir_parameter>  m_parameter_list;
  std::vector<ir_operation>  m_operation_list;

  std::list<ir_block_info>  m_block_info_list;

public:
  static constexpr std::string_view  m_first_label = "entry";

  ir_function() noexcept{}
  ir_function(ir_type_info  ti, std::string_view  name, std::vector<ir_parameter>&&  parals={}) noexcept:
  m_type_info(ti), m_name(name), m_parameter_list(std::move(parals)){}

  void  read(token_iterator&  it);

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  const std::vector<ir_parameter>&  get_parameter_list() const noexcept{return m_parameter_list;}
  const std::vector<ir_operation>&  get_operation_list() const noexcept{return m_operation_list;}

  const ir_block_info*  find_block_info(std::string_view  label) const noexcept;
  const ir_operation*    find_operation(std::string_view  label) const noexcept;

  ir_function&  add_label(const char*  fmt, ...) noexcept;

  std::string_view  get_last_label() const noexcept{
    return m_operation_list.back().get_label();
  }

  ir_operation&  add_operation() noexcept;

  void  finalize() noexcept;

  void  print() const noexcept;

};


class
ir_variable: public ir_value
{

public:
  ir_variable(ir_type_info  ti) noexcept: ir_value(ti){}
  ir_variable(const ir_value&   v) noexcept: ir_value(v){}
  ir_variable(      ir_value&&  v) noexcept: ir_value(std::move(v)){}

  template<class...  Args>
  ir_variable(Args&&...  args){assign(std::forward<Args>(args)...);}

  template<class...  Args>
  ir_variable&  operator=(Args&&...  args){return assign(std::forward<Args>(args)...);}

  ir_variable&  assign(const ir_value&  v);
  ir_variable&  assign(bool  b){return assign(static_cast<int64_t>(b? 1:0));}
  ir_variable&  assign(int64_t  i);
  ir_variable&  assign(double  f);
  ir_variable&  assign(std::string_view  sv);
  ir_variable&  assign(ir_address  addr){return assign(addr.get_word());}

};


class
ir_variable_info
{
  ir_type_info  m_type_info;

  std::string  m_name;

  ir_memory  m_memory;

public:
  ir_variable_info(ir_type_info  ti, std::string_view  name                  ) noexcept: m_type_info(ti), m_name(name){}
  ir_variable_info(ir_type_info  ti, std::string_view  name, ir_memory&&  mem) noexcept: m_type_info(ti), m_name(name), m_memory(std::move(mem)){}
  ir_variable_info(ir_value&&  v, std::string_view  name) noexcept: m_type_info(v.get_type_info()), m_name(name){m_memory = v.release_memory();}

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}
  const std::string&   get_name()      const noexcept{return m_name;}
  const ir_memory&     get_memory()    const noexcept{return m_memory;}

  void  print() const noexcept{
      if(m_memory)
      {
        ir_value  v(m_memory.get_string());

        v.print();
      }

    else
      {
        m_type_info.print();
      }


    printf(" %s",m_name.data());
  }

};


class
ir_context
{
  std::vector<ir_variable_info>  m_variable_info_list;

  std::vector<std::unique_ptr<ir_function>>  m_function_list;

  void  read_function(token_iterator&  it);
  void  read_variable(token_iterator&  it);

public:
  void  clear() noexcept{m_function_list.clear();}

  void  read(token_iterator&  it);
  void  read(std::string_view  sv);

  const std::vector<ir_variable_info>&  get_variable_info_list() const noexcept{return m_variable_info_list;}

  const std::vector<std::unique_ptr<ir_function>>&  get_function_list() const noexcept{return m_function_list;}

  ir_function&  create_function(ir_type_info  ret_ti, std::string_view  fn_name, std::vector<ir_parameter>&&  parals) noexcept;

  const ir_function*       find_function(std::string_view  label) const noexcept;
  const ir_variable_info*  find_variable_info(std::string_view  name) const noexcept;

  void  finalize() noexcept;

  void  print() const noexcept;

};


class
ir_link
{
  uint32_t  m_index=0;

  std::string  m_name;

public:
  ir_link() noexcept{}
  ir_link(uint32_t  i, std::string_view  name) noexcept: m_index(i), m_name(name){}

  uint32_t  get_index() const noexcept{return m_index;}

  const std::string&  get_name() const noexcept{return m_name;}

  void  print() const noexcept{printf("0x%08X %s",m_index,m_name.data());}

};


class
ir_processor
{
  std::vector<ir_variable>  m_variable_table;

  std::vector<ir_link>  m_global_link_table;

  const ir_context*  m_context;

  struct flags{
    static constexpr int  log = 1;
  };

  status_value<int>  m_status;

  struct frame{
    ir_address  m_return_value_address;

    const ir_function*  m_function=nullptr;

    std::vector<ir_link>  m_local_link_table;

    const ir_block_info*  m_previous_block_info=nullptr;
    const ir_block_info*  m_current_block_info=nullptr;

    const ir_operation*  m_current=nullptr;
    const ir_operation*  m_end=nullptr;

  };

  std::vector<frame>  m_frame_stack;

  void  operate(const ir_operation&  op);

  void  operate_ari(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var);
  void  operate_biw(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var);
  void  operate_cmp(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var);
  void  operate_ld(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var);
  void  operate_st(std::string_view  instr, const std::vector<ir_operand>&  opls);

  void  operate_cal(ir_address  retv_addr, const std::vector<ir_operand>&  opls);
  void  operate_br( const std::vector<ir_operand>&  opls);
  void  operate_phi(const std::vector<ir_phi_element>&  phels, ir_variable&  var);

  ir_value  evaluate(const ir_operand&  o);

  void  jump(std::string_view  label);
  void  jump(const ir_operation&  op);

  ir_address  find_variable(std::string_view  label) noexcept;

        ir_variable&  get_variable(ir_address  addr)       noexcept{return m_variable_table[addr.get_index()];}
  const ir_variable&  get_variable(ir_address  addr) const noexcept{return m_variable_table[addr.get_index()];}

        ir_variable&  get_variable(uint32_t  i)       noexcept{return m_variable_table[i];}
  const ir_variable&  get_variable(uint32_t  i) const noexcept{return m_variable_table[i];}

  void  pop_frame();

public:
  ir_processor() noexcept{}

  template<class...  Args>
  ir_processor(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  ir_processor&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  void  assign(const ir_context&  ctx);

  void  reset();

  void  call(ir_address  retv_addr, std::string_view  fn_name, std::vector<ir_value>&&  args);

  void  step();
  void   run();

  void   enable_log() noexcept{m_status.set(  flags::log);}
  void  disable_log() noexcept{m_status.unset(flags::log);}

  void  print_global_variables() const noexcept;
  void  print() const noexcept;

};




}




#endif




