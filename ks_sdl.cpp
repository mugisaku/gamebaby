#include"libgbstd/control.hpp"
#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/random.hpp"
#include"libsdlglu/sdl.hpp"




using namespace gbstd;


namespace{


const gbstd::clock*
g_clock;


gbstd::alarm
g_alarm;


timer
g_timer;


sprite
g_screen_sprite;





struct
space
{
  static constexpr int  m_size = 16;

  point  m_point;

  bool  m_erase=false;

  int  m_number=0;

  space*     m_up;
  space*   m_left;
  space*  m_right;
  space*   m_down;

  operator bool() const noexcept{return m_number;}

  bool  test_erase_flag() const noexcept{return m_erase;}

  space&    set_erase_flag() noexcept{  m_erase =  true;  return *this;}
  space&  unset_erase_flag() noexcept{  m_erase = false;  return *this;}

  int  get_number() const noexcept{return m_number;}

  color  get_color() const noexcept{return (m_number < 0)? colors::blue:colors::red;}

  void  draw(const canvas&  cv, int  state) const noexcept;

};


void
space::
draw(const canvas&  cv, int  state) const noexcept
{
  int  x = m_size*m_point.x;
  int  y = m_size*m_point.y;

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

  space  m_table[m_height][m_width];

  space*  m_current=&m_table[0][0];

  stage() noexcept;

  void  reset() noexcept;

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


constexpr int  g_screen_w = space::m_size*stage::m_width;
constexpr int  g_screen_h = space::m_size*stage::m_height;


void
stage::
reset() noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      sp.m_number = 0;
    }}


  m_status.clear();
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
          state = (!sp.test_erase_flag() || (m_blink_counter&1))? 1:0;
        }

      else
        {
          state = (m_current == &sp)? 2:1;
        }


      sp.draw(cv,state);
    }}


  auto  pt = m_current->m_point;

  char  buf[256];

  snprintf(buf,sizeof(buf),"x: %d, y: %d",pt.x,pt.y);

  cv.draw_string(colors::white,buf,0,0);

    if(m_status.test(flags::gameover))
    {
      cv.draw_string(colors::white,"GAME OVER",g_screen_w/2-(8*5),g_screen_h/2);
    }
}


stage
g_stage;




void
evaluate_to_down(space*  head) noexcept
{
    if(*head)
    {
      int  i = head->get_number();

      auto  tail = head->m_down;

        while(tail && *tail)
        {
          i += tail->get_number();

          tail = tail->m_down;
        }


        if(!i || (i == 10))
        {
          g_stage.m_status.set(stage::flags::erase);

            while(head != tail)
            { 
              head->set_erase_flag();
                
              head = head->m_down;
            }
        }
    }
}


void
evaluate_to_right(space*  head) noexcept
{
    if(*head)
    {
      int  i = head->get_number();

      auto  tail = head->m_right;

        while(tail && *tail)
        {
          i += tail->get_number();

          tail = tail->m_right;
        }


        if(!i || (i == 10))
        {
          g_stage.m_status.set(stage::flags::erase);

            while(head != tail)
            { 
              head->set_erase_flag();
                
              head = head->m_right;
            }
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

        if(sp.test_erase_flag())
        {
          sp.unset_erase_flag();

          sp.m_number = 0;
        }
    }}


    for(int  y = 0;  y < stage::m_height;  ++y){
    for(int  x = 0;  x < stage::m_width ;  ++x){
      fall(g_stage.m_table[stage::m_height-1-y][x]);
    }}


  g_stage.m_status.unset(stage::flags::chink);

  replace_execution(evaluate);
}


void
blink() noexcept
{
    if(g_alarm)
    {
      ++g_stage.m_blink_counter;

        if(g_stage.m_blink_counter >= 8)
        {
          replace_execution(chink);

          return;
        }

      else
        {
          g_alarm.reset();
        }
    }


  interrupt_execution();
}


void
evaluate() noexcept
{
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

      replace_execution(blink);
    }

  else
    {
      pop_execution();
    }
}


void
control_by_cursor(space*  next) noexcept
{
  auto&  here = *g_stage.m_current;

    if(!next || next->m_number)
    {
      replace_execution(evaluate);
    }

  else
    if(next)
    {
      next->m_number = here.m_number    ;
                       here.m_number = 0;

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


    if(get_keys().test_diu())
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


  interrupt_execution();
}


void
gameover() noexcept
{
    if(get_keys().test_opr())
    {
      barrier_keys(120);

      g_stage.reset();

      pop_execution();
    }


  interrupt_execution();
}


void
start() noexcept
{
  auto  start_sp = &g_stage.m_table[0][stage::m_width/2];

    if(start_sp->m_number)
    {
      g_stage.m_status.set(stage::flags::gameover);

      push_execution(gameover);
    }

  else
    {
      static uniform_rand  n_rand(1,4);

      g_stage.m_current = start_sp;

        for(;;)
        {
          auto  n = n_rand();

            if(n)
            {
              g_stage.m_current->m_number = n;

              break;
            }
        }


      push_execution(control_drop);
    }
}



void
main_loop() noexcept
{
  static uint32_t  next;

  sdl::update_control();

  step_execution();

    if((get_time() >= next))
    {
      redraw_screen();

      sdl::update_screen();

      next = get_time()+80;
    }
}


}




int
main(int  argc, char**  argv)
{
  allocate_clocks(1);
  allocate_timers(2);
  allocate_sprites(2);


  g_clock = &get_clock(0);

  g_alarm.assign(*g_clock).set_interval(100).reset();

//  get_timer(0).set_clock_pointer(clock).set_interval(1000).set_callback({g_taro,&user::drive});

  get_sprite(0).set_callback({g_stage,&stage::draw});

  push_execution(start);

  sdl::init(g_screen_w,g_screen_h);

  sdl::update_screen();

    for(;;)
    {
      main_loop();

      sdl::delay(20);
    }


  sdl::quit();

  return 0;
}




