#include"libgbstd/control.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/random.hpp"
#include"libgbstd/ilc.hpp"
#include"libsdlglu/sdl.hpp"


using namespace gbstd;


namespace{


uniform_rand  g_number_rand;
uniform_rand  g_color_rand;


gbstd::clock*
g_clock;


gbstd::alarm
g_alarm;


struct
space
{
  static constexpr int  m_size = 16;

  point  m_point;

  struct flags{
    static constexpr int  right_erase = 1;
    static constexpr int   down_erase = 2;

  };


  status_value<int>  m_status;

  int  m_number=0;

  color  m_color;

  space*     m_up;
  space*   m_left;
  space*  m_right;
  space*   m_down;

  operator bool() const noexcept{return m_number;}

  bool  test_right_erase_flag() const noexcept{return m_status.test(flags::right_erase);}
  bool   test_down_erase_flag() const noexcept{return m_status.test(flags::down_erase);}
  bool   test_erase_flags() const noexcept{return m_status;}

  space&    set_right_erase_flag() noexcept{  m_status.set(  flags::right_erase);  return *this;}
  space&  unset_right_erase_flag() noexcept{  m_status.unset(flags::right_erase);  return *this;}

  space&    set_down_erase_flag() noexcept{  m_status.set(  flags::down_erase);  return *this;}
  space&  unset_down_erase_flag() noexcept{  m_status.unset(flags::down_erase);  return *this;}

  space&  clear_erase_flags() noexcept{  m_status.clear();  return *this;}

  int  get_number() const noexcept{return m_number;}

  color  get_color() const noexcept{return m_color;}

  void  draw(const canvas&  cv, int  y_offset, int  state) const noexcept;

};


void
space::
draw(const canvas&  cv, int  y_offset, int  state) const noexcept
{
  int  x =          (m_size*m_point.x);
  int  y = y_offset+(m_size*m_point.y);

    if(m_number && state)
    {
      cv.fill_rectangle(get_color(),x+1,y+1,m_size-2,m_size-2);

      cv.draw_rectangle((state == 2)? colors::white:colors::gray,x,y,m_size,m_size);

      char  buf[2] = {static_cast<char>('0'+std::abs(m_number)),0};

      cv.draw_string(colors::white,buf,x+4,y-4);
    }

  else
    {
      cv.fill_rectangle(colors::black,x,y,m_size,m_size);
    }
}


struct
stage
{
  static constexpr int   m_width =  8;
  static constexpr int  m_height = 12;

  struct flags{
    static constexpr int     erase = 1;
    static constexpr int     chink = 2;
    static constexpr int  gameover = 4;
  };


  int  m_blink_counter;

  status_value<int>  m_status;

  int  m_chink_counter;

  uint32_t  m_temporary_score;
  uint32_t  m_total_score;

  space  m_table[m_height][m_width];

  space*  m_current=&m_table[0][0];

  stage() noexcept;

  void  reset() noexcept;

  void  add_temporary_score(int  v) noexcept{m_temporary_score += v*m_chink_counter*m_chink_counter;}

  void  draw(const canvas&  cv) noexcept;

};


stage::
stage() noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      if(x               ){sp.m_left  = &m_table[y  ][x-1];}
      if(y               ){sp.m_up    = &m_table[y-1][x  ];}
      if(x < (m_width -1)){sp.m_right = &m_table[y  ][x+1];}
      if(y < (m_height-1)){sp.m_down  = &m_table[y+1][x  ];}

      sp.m_point = {x,y};
    }}
}


constexpr int  g_screen_w =                   space::m_size*stage::m_width;
constexpr int  g_screen_h = (g_font_height*2)+space::m_size*stage::m_height;


void
stage::
reset() noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      sp.m_number = 0;
      sp.m_color  = colors::null;
    }}


  m_status.clear();

  m_chink_counter = 0;

  m_total_score = 0;
}


void
stage::
draw(const canvas&  cv) noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      int  state;

        if(m_status.test(flags::chink))
        {
          state = (!sp.test_erase_flags() || (m_blink_counter&1))? 1:0;
        }

      else
        {
          state = (m_current == &sp)? 2:1;
        }


      sp.draw(cv,g_font_height*2,state);
    }}


  auto  pt = m_current->m_point;

  char  buf[256];

  snprintf(buf,sizeof(buf),"SCORE: %5dちんちん",m_total_score);

  cv.draw_string(colors::white,buf,0,0);

    if(m_status.test(flags::gameover))
    {
      cv.draw_string(colors::white,"GAME OVER",g_screen_w/2-(8*5),g_screen_h/2);
    }

  else
    if(m_chink_counter)
    {
      snprintf(buf,sizeof(buf),"%dれんさ",m_chink_counter);

      cv.draw_string(colors::white,buf,0,g_font_height);
    }


  cv.draw_rectangle(colors::white,0,g_font_height*2,space::m_size*stage::m_width,space::m_size*stage::m_height);
}


stage
g_stage;




constexpr int  g_key_number = 6;
constexpr int  g_score_base = 10;


void
evaluate_to_down(space*  head) noexcept
{
    if(head->get_number() && !head->test_down_erase_flag())
    {
      int  i = head->get_number();

      auto  tail = head->m_down;

        while(tail && tail->get_number() && !tail->test_down_erase_flag() & (tail->get_color() == head->get_color()))
        {
          i += tail->get_number();

            if(!i || (i == g_key_number))
            {
              int  block_n = 1;

              g_stage.m_status.set(stage::flags::erase);

                while(head != tail)
                { 
                  ++block_n;

                  head->set_down_erase_flag();
                    
                  head = head->m_down;
                }


              g_stage.add_temporary_score(g_score_base*block_n);

              head->set_down_erase_flag();

              break;
            }

          else
            if(i > g_key_number)
            {
              break;
            }


          tail = tail->m_down;
        }
    }
}


void
evaluate_to_right(space*  head) noexcept
{
    if(head->get_number() && !head->test_right_erase_flag())
    {
      int  i = head->get_number();

      auto  tail = head->m_right;

        while(tail && tail->get_number() && !tail->test_right_erase_flag() & (tail->get_color() == head->get_color()))
        {
          i += tail->get_number();

            if(!i || (i == g_key_number))
            {
              int  block_n = 1;

              g_stage.m_status.set(stage::flags::erase);

                while(head != tail)
                { 
                  ++block_n;

                  head->set_right_erase_flag();
                    
                  head = head->m_right;
                }


              g_stage.add_temporary_score(g_score_base*block_n);

              head->set_right_erase_flag();

              break;
            }

          else
            if(i > g_key_number)
            {
              break;
            }


          tail = tail->m_right;
        }
    }
}


void
fall(space&  sp) noexcept
{
    if(sp)
    {
      auto  p = &sp;

        while(p->m_down && !*p->m_down)
        {
          p->m_down->m_number = p->m_number    ;
                                p->m_number = 0;

          p->m_down->m_color = p->m_color               ;
                               p->m_color = colors::null;

          p = p->m_down;
        }
    }
}


void  evaluate() noexcept;


void
chink() noexcept
{
    for(int  y = 0;  y < stage::m_height;  ++y){
    for(int  x = 0;  x < stage::m_width ;  ++x){
      auto&  sp = g_stage.m_table[y][x];

        if(sp.test_erase_flags())
        {
          sp.clear_erase_flags();

          sp.m_number = 0;
        }
    }}


    for(int  y = 0;  y < stage::m_height;  ++y){
    for(int  x = 0;  x < stage::m_width ;  ++x){
      fall(g_stage.m_table[stage::m_height-1-y][x]);
    }}


  g_stage.m_status.unset(stage::flags::chink);

  g_exec.replace(evaluate);
}


void
blink() noexcept
{
    if(g_alarm)
    {
      ++g_stage.m_blink_counter;

        if(g_stage.m_blink_counter >= 10)
        {
          g_exec.replace(chink);

          return;
        }

      else
        {
          g_alarm.reset();
        }
    }


  g_exec.interrupt();
}


void
evaluate() noexcept
{
  ++g_stage.m_chink_counter;

  g_stage.m_status.unset(stage::flags::erase);

    for(int  y = 0;  y < stage::m_height;  ++y){
    for(int  x = 0;  x < stage::m_width ;  ++x){
      auto  ptr = &g_stage.m_table[y][x];

      evaluate_to_right(ptr);
      evaluate_to_down( ptr);
    }}


    if(g_stage.m_status.test(stage::flags::erase))
    {
      g_stage.m_blink_counter = 0;

      g_alarm.reset();

      g_stage.m_status.set(stage::flags::chink);

      g_exec.replace(blink);
    }

  else
    {
      g_stage.m_chink_counter = 0;

      g_stage.m_total_score += g_stage.m_temporary_score    ;
                               g_stage.m_temporary_score = 0;

      g_exec.pop();
    }
}


void
control_by_cursor(space*  next) noexcept
{
  auto&  here = *g_stage.m_current;

    if(!next || next->m_number)
    {
      g_stage.m_temporary_score = 0;

      g_exec.replace(evaluate);
    }

  else
    if(next)
    {
      next->m_number = here.m_number    ;
                       here.m_number = 0;

      next->m_color = here.m_color               ;
                      here.m_color = colors::null;

      g_stage.m_current = next;
    }
}


void
control_drop() noexcept
{
  static constexpr uint32_t  delay = 120;

  auto&  here = *g_stage.m_current;

    if(get_keys().test_opr())
    {
      barrier_keys(120);
    }


    if(here.m_point.y && get_keys().test_diu())
    {
      barrier_keys(delay);

      control_by_cursor(here.m_up);
    }

  else
    if(get_keys().test_did())
    {
        if(here.m_point.x && get_keys().test_dil())
        {
          control_by_cursor(here.m_left);
        }

      else
        if((here.m_point.x < (g_stage.m_width-1)) && get_keys().test_dir())
        {
          control_by_cursor(here.m_right);
        }


      control_by_cursor(here.m_down);

      barrier_keys(delay/3);
    }

  else
    {
        if(here.m_point.x && get_keys().test_dil())
        {
          barrier_keys(delay);

          control_by_cursor(here.m_left);
        }

      else
        if((here.m_point.x < (g_stage.m_width-1)) && get_keys().test_dir())
        {
          barrier_keys(delay);

          control_by_cursor(here.m_right);
        }
    }


  g_exec.interrupt();
}


void
gameover() noexcept
{
    if(get_keys().test_opr())
    {
      barrier_keys(120);

      g_stage.reset();

      g_exec.pop();
    }


  g_exec.interrupt();
}


void
start() noexcept
{
  auto  start_sp = &g_stage.m_table[0][stage::m_width/2];

    if(start_sp->m_number)
    {
      g_stage.m_status.set(stage::flags::gameover);

      g_exec.push(gameover);
    }

  else
    {
      g_stage.m_current = start_sp;

        for(;;)
        {
          auto  n = g_number_rand();

            if(n)
            {
              g_stage.m_current->m_number = n;

              auto  cn = g_color_rand();

              g_stage.m_current->m_color = (cn == 0)? colors::yellow
                                          :(cn == 1)? colors::cyan
                                          :(cn == 2)? colors::magenta
                                          :(cn == 3)? colors::black
                                          :           colors::null;

              break;
            }
        }


      g_exec.push(control_drop);
    }
}


}




int
main(int  argc, char**  argv)
{
  sdl::init(g_screen_w,g_screen_h);

  auto&  root_dir = get_root_directory();

  auto&  clocks_dir = *root_dir.find_directory("clocks");

  g_clock = &clocks_dir.create_node_by_name("system")->be_clock();

  g_number_rand.reset(1,3);
  g_color_rand.reset(0,3);

  g_alarm.assign(*g_clock)
    .set_interval(100)
    .reset();

  root_dir.create_node_by_path("/video/sprites/spr00")
   ->be_sprite()
    .set_callback({g_stage,&stage::draw});

  g_exec.push(start);

  gbstd::set_caption("計算ゲーム");

  gbstd::set_description(
R"(ルール:<br>
置かれた同じ色のブロックが<br>
左から右、上から下に加算評価され、合計が6であるとき、消える)"
);

  gbstd::show_github_link();
  gbstd::show_twitter_link();

  sdl::update_screen();

  gbstd::push_execution(g_exec);

  sdl::start_loop();

  return 0;
}




