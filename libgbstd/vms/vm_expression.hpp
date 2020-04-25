

class
expression_error
{
public:
  std::string  m_comment;

  expression_error(std::string_view  comm="") noexcept: m_comment(comm){}

};


class   hot_object;
class tepid_object;
class  cold_object;


class
cold_object
{
  memory  m_memory;

  const type_info*  m_type_info=nullptr;

public:
  cold_object() noexcept{}
  cold_object(const tepid_object&  o) noexcept;
  cold_object(const   hot_object&  o) noexcept;
  cold_object(const type_info&  ti, address_t  addr) noexcept;
  cold_object(bool  b) noexcept;
  cold_object(  int8_t  i) noexcept;
  cold_object( int16_t  i) noexcept;
  cold_object( int32_t  i) noexcept;
  cold_object( int64_t  i) noexcept;
  cold_object(uint64_t  u) noexcept;
  cold_object(float   f) noexcept;
  cold_object(double  f) noexcept;
  cold_object(nullptr_t  ptr) noexcept;
  cold_object(const function&  fn) noexcept;
  cold_object(std::string_view  sv) noexcept;

  operator bool() const noexcept{return m_type_info;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  const type_info&  get_type_info() const noexcept{return *m_type_info;}

   int64_t  get_integer() const noexcept;

  double  get_fpn() const noexcept;

  int  get_size() const noexcept{return m_type_info->get_size();}

  void  print() const noexcept;

};


class
tepid_object: public cold_object
{
  memory  m_base_memory;

public:
  tepid_object() noexcept{}
  tepid_object(const hot_object&   ho) noexcept;
  tepid_object(const cold_object&  co, const memory&  base_mem) noexcept;

        memory&  get_base_memory()       noexcept{return m_base_memory;}
  const memory&  get_base_memory() const noexcept{return m_base_memory;}

  void  print() const noexcept;

};


class
hot_object
{
  memory  m_memory;

  const type_info*  m_type_info=nullptr;

  address_t  m_address=0;

public:
  hot_object() noexcept{}
  hot_object(const tepid_object&  o) noexcept;
  hot_object(const memory&  mem, const type_info&  ti, address_t  addr) noexcept:
  m_memory(mem), m_type_info(&ti), m_address(addr){}

  operator bool() const noexcept{return m_type_info;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  const type_info&  get_type_info() const noexcept{return *m_type_info;}

  address_t  get_address() const noexcept{return m_address;}

  int  get_size() const noexcept{return m_type_info->get_size();}

   int64_t  get_integer()          const noexcept;
  uint64_t  get_unsigned_integer() const noexcept;

  double  get_fpn() const noexcept;

  hot_object  get_struct_member(std::string_view  name) const noexcept;
  hot_object  get_union_member(std::string_view  name) const noexcept;
  hot_object  get_element(int  i) const noexcept;

  tepid_object  write(cold_object  co) noexcept;
	
};




namespace operations{
cold_object      bit_not(cold_object  o) noexcept;
cold_object  logical_not(cold_object  o) noexcept;
cold_object          neg(cold_object  o) noexcept;
cold_object         size(cold_object  o) noexcept;
cold_object      address(hot_object  o) noexcept;
tepid_object   dereference(cold_object  o, const memory&  home_mem) noexcept;
cold_object      postfix_increment(hot_object  o) noexcept;
cold_object      postfix_decrement(hot_object  o) noexcept;

cold_object      add(cold_object  lo, cold_object  ro) noexcept;
cold_object      sub(cold_object  lo, cold_object  ro) noexcept;
cold_object      mul(cold_object  lo, cold_object  ro) noexcept;
cold_object      div(cold_object  lo, cold_object  ro) noexcept;
cold_object      rem(cold_object  lo, cold_object  ro) noexcept;
cold_object      shl(cold_object  lo, cold_object  ro) noexcept;
cold_object      shr(cold_object  lo, cold_object  ro) noexcept;
cold_object   bit_or(cold_object  lo, cold_object  ro) noexcept;
cold_object  bit_and(cold_object  lo, cold_object  ro) noexcept;
cold_object  bit_xor(cold_object  lo, cold_object  ro) noexcept;

tepid_object  assign(hot_object  lo, cold_object  ro) noexcept;

cold_object           eq(cold_object  lo, cold_object  ro) noexcept;
cold_object          neq(cold_object  lo, cold_object  ro) noexcept;
cold_object           lt(cold_object  lo, cold_object  ro) noexcept;
cold_object         lteq(cold_object  lo, cold_object  ro) noexcept;
cold_object           gt(cold_object  lo, cold_object  ro) noexcept;
cold_object         gteq(cold_object  lo, cold_object  ro) noexcept;
cold_object   logical_or(cold_object  lo, cold_object  ro) noexcept;
cold_object  logical_and(cold_object  lo, cold_object  ro) noexcept;

tepid_object  subscript(cold_object  lo, cold_object  ro, const memory&  mem) noexcept;
tepid_object      arrow(cold_object  lo, const expression&  r, const memory&  mem) noexcept;
tepid_object        dot( hot_object  lo, const expression&  r) noexcept;
tepid_object     invoke(cold_object  lo, const expression&  r) noexcept;


}




class
mutability
{
  bool  m_value;

public:
  explicit constexpr mutability(bool  v=false) noexcept: m_value(v){}

  operator bool() const noexcept{return m_value;}

};


namespace mutabilities{
constexpr auto   mute = mutability( true);
constexpr auto  unmute = mutability(false);
}


using mutabilities::mute;
using mutabilities::unmute;


struct undefined{};

class
identifier
{
  std::string  m_string;

public:
  identifier() noexcept{}
  identifier(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  std::string  release() noexcept{return std::move(m_string);}

};


class expression;


class
prefix_unary_operation
{
protected:
  operator_code  m_opcode;

  std::unique_ptr<expression>  m_operand;

public:
  prefix_unary_operation(operator_code  op, std::unique_ptr<expression>&&  o) noexcept:
  m_opcode(op), m_operand(std::move(o)){}

  prefix_unary_operation(const prefix_unary_operation&  rhs) noexcept{assign(rhs);}

  prefix_unary_operation&  operator=(const prefix_unary_operation&  rhs) noexcept{return assign(rhs);}
  prefix_unary_operation&  assign(const prefix_unary_operation&  rhs) noexcept;

  operator_code  get_opcode() const noexcept{return m_opcode;}

        expression&  get_operand()       noexcept{return *m_operand;}
  const expression&  get_operand() const noexcept{return *m_operand;}

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

};


class
postfix_unary_operation: public prefix_unary_operation
{
public:
  using prefix_unary_operation::prefix_unary_operation;

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

};


class
binary_operation
{
  operator_code  m_opcode;

  std::unique_ptr<expression>  m_left;
  std::unique_ptr<expression>  m_right;

public:
  binary_operation(operator_code  op, std::unique_ptr<expression>&&  l, std::unique_ptr<expression>&&  r) noexcept:
  m_opcode(op), m_left(std::move(l)), m_right(std::move(r)){}

  binary_operation(const binary_operation&  rhs) noexcept{assign(rhs);}

  binary_operation&  operator=(const binary_operation&  rhs) noexcept{return assign(rhs);}
  binary_operation&  assign(const binary_operation&  rhs) noexcept;

  operator_code  get_opcode() const noexcept{return m_opcode;}

        expression&  get_left()       noexcept{return *m_left;}
  const expression&  get_left() const noexcept{return *m_left;}

        expression&  get_right()       noexcept{return *m_right;}
  const expression&  get_right() const noexcept{return *m_right;}

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

};


class
expression
{
  enum class kind{
    null,

    identifier,
    undefined_literal,
    null_pointer_literal,
    boolean_literal,
    string_literal,
    integer_literal,
    fpn_literal,

     prefix_unary,
    postfix_unary,
    binary,

  } m_kind=kind::null;

  union data{
    bool         b;
    std::string  s;
    int64_t      i;

    double  f;

     prefix_unary_operation   pre_un;
    postfix_unary_operation   pos_un;

    binary_operation  bin;

  data() noexcept{}
 ~data(){}

  } m_data;

  address_t  m_begin_address=0;
  address_t  m_end_address=0;

public:
  expression() noexcept{}
  expression(const expression&   rhs) noexcept{assign(rhs);}
  expression(      expression&&  rhs) noexcept{assign(std::move(rhs));}
  expression(undefined  u) noexcept{assign(u);}
  expression(nullptr_t  n) noexcept{assign(n);}
  expression(bool  b) noexcept{assign(b);}
  expression(identifier&&  id) noexcept{assign(std::move(id));}
  expression(std::string_view  sv) noexcept{assign(sv);}
  expression(int64_t  i) noexcept{assign(i);}
  expression(double  f) noexcept{assign(f);}
  expression(prefix_unary_operation&&  op) noexcept{assign(std::move(op));}
  expression(postfix_unary_operation&&  op) noexcept{assign(std::move(op));}
  expression(binary_operation&&  op) noexcept{assign(std::move(op));}
 ~expression(){clear();}

  expression&  operator=(const expression&   rhs) noexcept{return assign(rhs);}
  expression&  operator=(      expression&&  rhs) noexcept{return assign(std::move(rhs));}
  expression&  operator=(undefined  u) noexcept{return assign(u);}
  expression&  operator=(nullptr_t  n) noexcept{return assign(n);}
  expression&  operator=(bool  b) noexcept{return assign(b);}
  expression&  operator=(identifier&&  id) noexcept{return assign(std::move(id));}
  expression&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  expression&  operator=(int64_t  i) noexcept{return assign(i);}
  expression&  operator=(double  f) noexcept{return assign(f);}
  expression&  operator=(prefix_unary_operation&&  op) noexcept{return assign(std::move(op));}
  expression&  operator=(postfix_unary_operation&&  op) noexcept{return assign(std::move(op));}
  expression&  operator=(binary_operation&&  op) noexcept{return assign(std::move(op));}

  operator bool() const noexcept{return m_kind != kind::null;}

  expression&  assign(const expression&   rhs) noexcept;
  expression&  assign(      expression&&  rhs) noexcept;
  expression&  assign(undefined  u) noexcept;
  expression&  assign(nullptr_t  n) noexcept;
  expression&  assign(bool  b) noexcept;
  expression&  assign(identifier&&  id) noexcept;
  expression&  assign(std::string_view  sv) noexcept;
  expression&  assign(int64_t  i) noexcept;
  expression&  assign(double  f) noexcept;
  expression&  assign(prefix_unary_operation&&  op) noexcept;
  expression&  assign(postfix_unary_operation&&  op) noexcept;
  expression&  assign(binary_operation&&  op) noexcept;

  expression&  clear() noexcept;

  address_t  get_begin_address() const noexcept{return m_begin_address;}
  address_t  get_end_address()   const noexcept{return m_end_address;}

  const std::string&  get_string() const noexcept{return m_data.s;}

  bool      get_boolean() const noexcept{return m_data.b;}
  int64_t   get_integer() const noexcept{return m_data.i;}
  double    get_fpn()     const noexcept{return m_data.f;}

  const prefix_unary_operation&   get_prefix_unary_operation() const noexcept{return m_data.pre_un;}
  const postfix_unary_operation&  get_postfix_unary_operation() const noexcept{return m_data.pos_un;}
  const binary_operation&  get_binary_operation() const noexcept{return m_data.bin;}

  bool  is_undefined_literal()    const noexcept{return m_kind == kind::undefined_literal;}
  bool  is_boolean_literal()      const noexcept{return m_kind == kind::boolean_literal;}
  bool  is_null_pointer_literal() const noexcept{return m_kind == kind::null_pointer_literal;}
  bool  is_identifier()           const noexcept{return m_kind == kind::identifier;}
  bool  is_string_literal()       const noexcept{return m_kind == kind::string_literal;}
  bool  is_integer_literal()      const noexcept{return m_kind == kind::integer_literal;}
  bool  is_fpn_literal()          const noexcept{return m_kind == kind::fpn_literal;}
  bool  is_prefix_unary()  const noexcept{return m_kind == kind::prefix_unary;}
  bool  is_postfix_unary() const noexcept{return m_kind == kind::postfix_unary;}
  bool  is_binary()        const noexcept{return m_kind == kind::binary;}

  tepid_object  evaluate(context&  ctx) const noexcept;

  address_t  allocate_address() noexcept;

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

  std::vector<const expression*>  get_argument_source_list() const noexcept;

  void  print() const noexcept;

};


expression  make_expression(token_iterator&  it);
expression  make_expression(std::string_view  sv);




