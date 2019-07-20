#ifndef gbstd_menu_hpp_is_included
#define gbstd_menu_hpp_is_included


#include"libgbstd/utility.hpp"
#include"libgbstd/image.hpp"
#include<vector>




namespace gbstd{
namespace windows{


class
style
{
  int     m_frame_top_width=0;
  int  m_frame_bottom_width=0;

  int   m_frame_left_width=0;
  int  m_frame_right_width=0;

public:
  style&  set_frame_top_width(   int  n) noexcept{  m_frame_top_width    = n;  return *this;}
  style&  set_frame_bottom_width(int  n) noexcept{  m_frame_bottom_width = n;  return *this;}
  style&  set_frame_left_width(  int  n) noexcept{  m_frame_left_width   = n;  return *this;}
  style&  set_frame_right_width( int  n) noexcept{  m_frame_right_width  = n;  return *this;}

  int  get_frame_top_width(   ) const noexcept{return m_frame_top_width   ;}
  int  get_frame_bottom_width() const noexcept{return m_frame_bottom_width;}
  int  get_frame_left_width(  ) const noexcept{return m_frame_left_width  ;}
  int  get_frame_right_width( ) const noexcept{return m_frame_right_width ;}

};


class stack;


class
result
{
  int  m_opening_value=0;
  int  m_closing_value=0;

public:
  result(int  op=0, int  cl=0) noexcept:
  m_opening_value(op), m_closing_value(cl){}

  int  get_opening_value() const noexcept{return m_opening_value;}
  int  get_closing_value() const noexcept{return m_closing_value;}

};


class
object
{
  friend class stack;

protected:
  struct flags{
    static constexpr int  open = 1;
    static constexpr int  show = 2;
  };

  status_value<int>  m_status;

  point  m_position;

  int  m_content_width =0;
  int  m_content_height=0;

  style  m_style;

public:
          object() noexcept{}
 virtual ~object(){}

  bool  is_opened() const noexcept{return m_status.test(flags::open);}
  bool  is_shown()  const noexcept{return m_status.test(flags::show);}

  object&  show() noexcept{  m_status.set(  flags::show);  return *this;}
  object&  hide() noexcept{  m_status.unset(flags::show);  return *this;}

  object&  set_x_position(int  n) noexcept{  m_position.x = n;  return *this;}
  object&  set_y_position(int  n) noexcept{  m_position.y = n;  return *this;}

  int  get_x_position() const noexcept{return m_position.x;}
  int  get_y_position() const noexcept{return m_position.y;}

  int  get_width()  const noexcept;
  int  get_height() const noexcept;

  int  get_content_width()  const noexcept{return m_content_width ;}
  int  get_content_height() const noexcept{return m_content_height;}

  object&  set_content_width( int  v) noexcept{  m_content_width  = v;  return *this;}
  object&  set_content_height(int  v) noexcept{  m_content_height = v;  return *this;}

  int  get_content_x_position() const noexcept{return m_position.x+get_style().get_frame_left_width();}
  int  get_content_y_position() const noexcept{return m_position.y+get_style().get_frame_top_width();}

  canvas  get_content_canvas(const canvas&  base_cv) const noexcept;
  canvas      get_frame_top_canvas(const canvas&  base_cv) const noexcept;
  canvas     get_frame_left_canvas(const canvas&  base_cv) const noexcept;
  canvas    get_frame_right_canvas(const canvas&  base_cv) const noexcept;
  canvas   get_frame_bottom_canvas(const canvas&  base_cv) const noexcept;
  canvas  get_frame_content_canvas(const canvas&  base_cv) const noexcept;

        style&  get_style()       noexcept{return m_style;}
  const style&  get_style() const noexcept{return m_style;}

  virtual void  render_frame(  const canvas&  cv) noexcept{}
  virtual void  render_content(const canvas&  cv) noexcept{}

  virtual void  work(result  res) noexcept{}
  virtual void  work(stack&  stk) noexcept{}

};


class
stack: public object
{
  struct element{
    int  m_opening_value;

    object*  m_object;

    std::vector<object*>  m_children;

    element() noexcept{}
    element(int  opval, object&  obj) noexcept: m_opening_value(opval), m_object(&obj){}

  };


  std::vector<element>  m_container;

public:
  ~stack(){clear();}

  stack&  clear() noexcept;

  operator bool() const noexcept{return m_container.size();}

  int  get_number_of_elements() const noexcept{return m_container.size();}

  stack&  open(int  opening_value, object&  obj) noexcept;

  stack&  add_child(object&  obj) noexcept;

  stack&  close_top(int  closing_value) noexcept;

  int  get_opening_value() const noexcept{return m_container.back().m_opening_value;}

  void  render_content(const canvas&  cv) noexcept override;

  void  work(stack&  stk) noexcept override;

  static void  draw(const gbstd::canvas&  cv, stack&  stk) noexcept;
  static void  tick(                          stack&  stk) noexcept;

};




}}




namespace gbstd{
namespace menus{




class
cell
{
  std::u16string  m_string;

  int  m_index=0;

  point  m_position;

  bool  m_active_flag=false;
  bool  m_selected_flag=false;

public:
  cell() noexcept{}

  cell(int  i, bool  active_flag=true) noexcept:
  m_index(i), m_active_flag(active_flag){}

  int    get_index(      ) const noexcept{return m_index;}
  cell&  set_index(int  i)       noexcept{  m_index = i;  return *this;}

  point  get_position(          ) const noexcept{return m_position;}
  cell&  set_position(point  pos)       noexcept{  m_position = pos;  return *this;}

  const std::u16string&   get_string(                       ) const noexcept{return m_string;}
  cell&                   set_string(std::u16string_view  sv)       noexcept{  m_string = sv;  return *this;}

  operator bool() const noexcept{return m_active_flag;}

  bool  test_active_flag() const noexcept{return m_active_flag;}

  cell&    set_active_flag() noexcept{  m_active_flag =  true;  return *this;}
  cell&  unset_active_flag() noexcept{  m_active_flag = false;  return *this;}

  bool  is_selected() const noexcept{return m_selected_flag;}

  cell&    select() noexcept{  m_selected_flag =  true;  return *this;}
  cell&  unselect() noexcept{  m_selected_flag = false;  return *this;}

};


class
table
{
  std::vector<cell>  m_container;

  int   m_width =0;
  int   m_height=0;

  int   m_cell_width =0;
  int   m_cell_height=0;

  void  (*m_callback)(const cell&,const canvas&);

public:
  table() noexcept{}

  table&  resize(int  w, int  h) noexcept;

  table&  set_cell_width( int  n) noexcept{  m_cell_width  = n;  return *this;}
  table&  set_cell_height(int  n) noexcept{  m_cell_height = n;  return *this;}

  table&  set_callback(void  (*cb)(const cell&,const canvas&)) noexcept{  m_callback = cb;  return *this;}

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int  get_cell_width()  const noexcept{return m_cell_width ;}
  int  get_cell_height() const noexcept{return m_cell_height;}

        cell*  get_cell_pointer(int  x, int  y)       noexcept{return &m_container[m_width*y+x];}
  const cell*  get_cell_pointer(int  x, int  y) const noexcept{return &m_container[m_width*y+x];}

  void  render(int  x, int  y, int  w, int  h, const gbstd::canvas&  cv) noexcept;

};




class
view
{
  table*  m_table=nullptr;

  int   m_width =0;
  int   m_height=0;

  gbstd::point  m_offset;

public:
  view() noexcept{}
  view(table&  tbl) noexcept{set_table(tbl);}

  view&  set_table(table&  tbl) noexcept;

  view&  set_width( int  v) noexcept{  m_width  = v;  return *this;}
  view&  set_height(int  v) noexcept{  m_height = v;  return *this;}

        table&  get_table()       noexcept{return *m_table;}
  const table&  get_table() const noexcept{return *m_table;}

  int  get_x_offset() const noexcept{return m_offset.x;}
  int  get_y_offset() const noexcept{return m_offset.y;}

  const point&  get_offset() const noexcept{return m_offset;}

  view&  set_x_offset(int  n) noexcept;
  view&  set_y_offset(int  n) noexcept;
  view&  add_x_offset(int  n) noexcept{return set_x_offset(m_offset.x+n);}
  view&  add_y_offset(int  n) noexcept{return set_y_offset(m_offset.y+n);}

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  cell&  get_cell(int  x, int  y) const noexcept{return *m_table->get_cell_pointer(m_offset.x+x,m_offset.y+y);}

  void  render(const gbstd::canvas&  cv) noexcept;

};


class
basic_menu_window: public windows::object
{
  view*  m_view;

  point  m_cursor_position;

public:
  basic_menu_window&  set_view(view&  v) noexcept;

  void  render_content(const canvas&  cv) noexcept override;

};


}}




#endif




