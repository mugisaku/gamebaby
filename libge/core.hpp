#ifndef ge_core_hpp_was_included
#define ge_core_hpp_was_included


#include"libgbstd/widget.hpp"
#include"libgbstd/file_op.hpp"


namespace ge{




class
drawing_recorder
{
  struct dot{
    gbstd::color  color;
    uint16_t  x;
    uint16_t  y;

    dot(gbstd::color  color_=gbstd::color(), int  x_=0, int  y_=0) noexcept:
    color(color_), x(x_), y(y_){}

  };


  std::vector<dot>  m_dot_buffer;

  struct record;

  record*  m_record_list=nullptr;

  uint32_t  m_number_of_records=0;

public:
  drawing_recorder() noexcept{}
  drawing_recorder(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder(      drawing_recorder&&  rhs) noexcept{*this = std::move(rhs);}
 ~drawing_recorder(){clear();}

  drawing_recorder&  operator=(const drawing_recorder&   rhs) noexcept=delete;
  drawing_recorder&  operator=(      drawing_recorder&&  rhs) noexcept;

  void  put(gbstd::color  color, int  x, int  y) noexcept{m_dot_buffer.emplace_back(color,x,y);}

  bool  rollback(const gbstd::canvas&  cv) noexcept;

  void  clear() noexcept;

  bool  push(bool  solid) noexcept;

  uint32_t  get_number_of_records() const noexcept{return m_number_of_records;}

};




class core;


struct
background_style
{
  gbstd::color  first_color ;
  gbstd::color  second_color;

};


class
core_source
{
  core&  m_core;

  gbstd::image  m_image;

  int  m_cell_width =0;
  int  m_cell_height=0;

  gbstd::widgets::label&  m_size_label;

  gbstd::widgets::menu_handler*  m_menu=nullptr;

public:
  core_source(gbstd::widgets::operating_node&  root, core&  cor) noexcept;

  core_source&  set_cell_size(int  cell_w, int  cell_h) noexcept;

  core_source&  inflate(int  w, int  h) noexcept;

  int  get_cell_width()  const noexcept{return m_cell_width ;}
  int  get_cell_height() const noexcept{return m_cell_height;}

  gbstd::canvas  get_canvas(gbstd::point  pt) noexcept;

  core_source&  load(const std::vector<uint8_t>&  bin) noexcept;

  core_source&  set_menu(gbstd::widgets::menu_handler&  mnu) noexcept{  m_menu = &mnu;  return *this;}

  core_source&  update_size_label() noexcept;

  gbstd::widgets::node&  get_size_label() noexcept{return m_size_label;}

  gbstd::widgets::node&  create_save_png_widget(gbstd::widgets::operating_node&  root) noexcept;
  gbstd::widgets::node&  create_save_txt_widget(gbstd::widgets::operating_node&  root) noexcept;
  gbstd::widgets::node&  create_extend_widget(gbstd::widgets::operating_node&  root) noexcept;

};


class
core_paint
{
  core&  m_core;

  enum class mode{
    draw_dot,
    draw_line,
    draw_rectangle,
    fill_rectangle,
    fill_area,
    select,
    paste,
    layer,

  } m_mode=mode::draw_dot;


  drawing_recorder  m_recorder;

  gbstd::color  m_drawing_color=gbstd::color(0,0,0);

  int  m_pointing_count=0;

  gbstd::point        m_drawing_point;
  gbstd::point  m_saved_drawing_point;

  gbstd::rectangle  m_operation_rect;

  gbstd::image  m_clipped_image;

  bool  m_drawing_is_fixed=true;

  gbstd::widgets::label&  m_brush_label;

  void  try_to_push_solid_record()    noexcept;
  void  try_to_push_nonsolid_record() noexcept;

  void  undo() noexcept;

  void  modify_dot(const gbstd::canvas&  cv, gbstd::color  color, gbstd::point  pt) noexcept;

  void  draw_line(gbstd::color  color                        ) noexcept;
  void  draw_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept;
  void  fill_rect(gbstd::color  color, gbstd::rectangle  rect) noexcept;
  void  fill_area(gbstd::color  color                        ) noexcept;

public:
  core_paint(gbstd::widgets::operating_node&  root, core&  cor) noexcept;

  core_paint&  reset() noexcept;

  core_paint&  cancel_drawing() noexcept;
  core_paint&  cancel_select() noexcept;

  core_paint&  revolve(             ) noexcept;
  core_paint&  reverse_horizontally() noexcept;
  core_paint&  reverse_vertically(  ) noexcept;
  core_paint&  mirror_vertically(   ) noexcept;
  core_paint&  shift_up(   bool  rotate) noexcept;
  core_paint&  shift_left( bool  rotate) noexcept;
  core_paint&  shift_right(bool  rotate) noexcept;
  core_paint&  shift_down( bool  rotate) noexcept;

  core_paint&  take_copy() noexcept;
  core_paint&      paste(bool  layer) noexcept;

  core_paint&   set_drawing_point(gbstd::point  pt)       noexcept{       m_drawing_point = pt;  return *this;}
  gbstd::point  get_drawing_point(                ) const noexcept{return m_drawing_point     ;}

  gbstd::color&  get_drawing_color() noexcept{return m_drawing_color;}

  const gbstd::pixel&  get_current_pixel() const noexcept;

  gbstd::rectangle  get_operation_rectangle() const noexcept{return m_operation_rect;}

  core_paint&  change_mode_to_draw_dot()       noexcept{  m_mode = mode::draw_dot;  return *this;}
  core_paint&  change_mode_to_draw_line()      noexcept{  m_mode = mode::draw_line;  return *this;}
  core_paint&  change_mode_to_draw_rectangle() noexcept{  m_mode = mode::draw_rectangle;  return *this;}
  core_paint&  change_mode_to_fill_rectangle() noexcept{  m_mode = mode::fill_rectangle;  return *this;}
  core_paint&  change_mode_to_fill_area()      noexcept{  m_mode = mode::fill_area;  return *this;}
  core_paint&  change_mode_to_select()         noexcept{  m_mode = mode::select;  return *this;}
  core_paint&  change_mode_to_paste()          noexcept{  m_mode = mode::paste;  return *this;}
  core_paint&  change_mode_to_layer()          noexcept{  m_mode = mode::layer;  return *this;}
 
  core_paint&  update_brush_label() noexcept;

  core_paint&  operator()() noexcept;


  gbstd::widgets::node&  create_operation_widget(gbstd::widgets::operating_node&  root) noexcept;
  gbstd::widgets::node&  create_tool_widget(gbstd::widgets::operating_node&  root) noexcept;

  gbstd::widgets::node&  create_save_button(gbstd::widgets::operating_node&  root, std::string_view  mode) noexcept;

  gbstd::widgets::node&  get_brush_label() noexcept{return m_brush_label;}

};


class
core_display
{
  core&  m_core;

  int  m_pixel_size=1;

  std::vector<gbstd::point>  m_underlay_stack;

  background_style  m_bg_style;

  static void   first_callback(gbstd::widgets::button_event  evt) noexcept;
  static void  second_callback(gbstd::widgets::button_event  evt) noexcept;

public:
  core_display(gbstd::widgets::operating_node&  root, core&  cor) noexcept;

  void  push_underlay(gbstd::point  pt) noexcept;
  void   pop_underlay() noexcept;
  int  get_number_of_underlays() const noexcept{return m_underlay_stack.size();}

  void   clear_underlay_stack() noexcept;

  int            get_pixel_size(      ) const noexcept{return m_pixel_size    ;}
  core_display&  set_pixel_size(int  n)       noexcept{       m_pixel_size = n;  return *this;}

  void                     set_background_style(background_style  bgst)       noexcept{       m_bg_style = bgst;}
  const background_style&  get_background_style(                      ) const noexcept{return m_bg_style       ;}

  void  render_rect(gbstd::rectangle  rect, const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;

  void  render_grid(      const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;
  void  render_underlay(  const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;
  void  render_background(const gbstd::canvas&  cv, int  pixel_size=0) const noexcept;
  void  render_canvas(const gbstd::canvas&  src_cv, const gbstd::canvas&  dst_cv, int  pixel_size=0) const noexcept;

  gbstd::widgets::node&  create_background_widget(gbstd::widgets::operating_node&  root) noexcept;

};




class
core: public gbstd::widgets::node
{
  struct flags{
    static constexpr int  grid     = 1;
    static constexpr int  underlay = 2;
  };


  gbstd::status_value<int>  m_status=3;

  core_source   m_source;
  core_paint    m_paint;
  core_display  m_display;

  gbstd::point  m_pixel_base_point;

  bool  m_focus=false;

public:
  core(gbstd::widgets::operating_node&  root) noexcept;

  core_source&   get_source()  noexcept{return m_source;}
  core_paint&    get_paint()   noexcept{return m_paint;}
  core_display&  get_display() noexcept{return m_display;}

  gbstd::canvas  get_canvas() noexcept{return m_source.get_canvas(m_pixel_base_point);}

  gbstd::point  get_pixel_base_point(                ) const noexcept{return m_pixel_base_point;}
  core&         set_pixel_base_point(gbstd::point  pt)       noexcept;

  core&  add_underlay() noexcept{  m_display.push_underlay(m_pixel_base_point);  return *this;}

  core&      show_grid() noexcept{  m_status.set(  flags::grid);  return *this;}
  core&      hide_grid() noexcept{  m_status.unset(flags::grid);  return *this;}
  core&  show_underlay() noexcept{  m_status.set(  flags::underlay);  return *this;}
  core&  hide_underlay() noexcept{  m_status.unset(flags::underlay);  return *this;}

  bool  test_underlay() const noexcept{return m_status.test(flags::underlay);}

  void  on_mouse_enter() noexcept override;
  void  on_mouse_leave() noexcept override;

  void  on_mouse_act(gbstd::point  mouse_pos) noexcept override;

  void  process_before_reform() noexcept override;

  void  render(const gbstd::canvas&  cv) noexcept override;

  gbstd::widgets::node&  create_underlay_widget(gbstd::widgets::operating_node&  root) noexcept;

};


}


#endif



