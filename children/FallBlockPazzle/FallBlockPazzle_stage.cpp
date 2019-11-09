#include"children/FallBlockPazzle/FallBlockPazzle.hpp"




namespace FallBlockPazzle{




stage::
stage() noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      sp.m_left  = (x               )? &m_table[y  ][x-1]:nullptr;
      sp.m_up    = (y               )? &m_table[y-1][x  ]:nullptr;
      sp.m_right = (x < (m_width -1))? &m_table[y  ][x+1]:nullptr;
      sp.m_down  = (y < (m_height-1))? &m_table[y+1][x  ]:nullptr;

      sp.m_point = {x,y};
    }}


  reset();
}


void
stage::
reset() noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      sp.m_number = 0;
      sp.m_color  = gbstd::colors::null;
    }}


  m_status.clear();

  m_chink_counter = 0;

  m_total_score = 0;
}


void
stage::
evaluate_to(space*  head, const space*  end, int  flag, int  increment) noexcept
{
    if(head->get_number() && !head->test_flag(flag))
    {
      int  i = head->get_number();

      auto  tail = head+increment;

        while((tail != end) && tail->get_number() && !tail->test_flag(flag) & (tail->get_color() == head->get_color()))
        {
          i += tail->get_number();

            if(!i || (i == m_key_number))
            {
              int  block_n = 1;

              m_status.set(flags::erase);

                while(head != tail)
                { 
                  ++block_n;

                  head->set_flag(flag);
                    
                  head += increment;
                }


              add_temporary_score(m_score_base*block_n);

              head->set_flag(flag);

              break;
            }

          else
            if(i > m_key_number)
            {
              break;
            }


          tail += increment;
        }
    }
}


void
fall(space&  sp) noexcept
{
    if(sp)
    {
      auto  p = &sp;

        while(p->get_down() && !*p->get_down())
        {
          p->get_down()->set_number(p->get_number( ));
                                    p->set_number(0) ;

          p->get_down()->set_color(p->get_color(                   ));
                                   p->set_color(gbstd::colors::null) ;

          p = p->get_down();
        }
    }
}


void
stage::
chink(gbstd::execution&  exec) noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x < m_width ;  ++x){
      auto&  sp = m_table[y][x];

        if(sp.test_flag(space::flags::erase))
        {
          sp.unset_flag(space::flags::erase);

          sp.m_number = 0;
        }
    }}


    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x < m_width ;  ++x){
      fall(m_table[m_height-1-y][x]);
    }}


  m_status.unset(flags::chink);

  exec.replace({*this,&stage::evaluate});
}


void
stage::
blink(gbstd::execution&  exec) noexcept
{
    if(m_alarm)
    {
      ++m_blink_counter;

        if(m_blink_counter >= 10)
        {
          exec.replace({*this,&stage::chink});

          return;
        }

      else
        {
          m_alarm.reset();
        }
    }


  exec.interrupt();
}


void
stage::
evaluate(gbstd::execution&  exec) noexcept
{
  ++m_chink_counter;

  m_status.unset(flags::erase);

    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x < m_width ;  ++x){
      auto  ptr = &m_table[y][x];

      evaluate_to(ptr,&m_table[       y][m_width],space::flags::right_erase,      1);
      evaluate_to(ptr,&m_table[m_height][      x],space::flags::down_erase ,m_width);
    }}


    if(m_status.test(flags::erase))
    {
      m_blink_counter = 0;

      m_alarm.reset();

      m_status.set(flags::chink);

      exec.replace({*this,&stage::blink});
    }

  else
    {
      m_chink_counter = 0;

      m_total_score += m_temporary_score    ;
                       m_temporary_score = 0;

      exec.pop();
    }
}


void
stage::
control_by_cursor(gbstd::execution&  exec, space*  next) noexcept
{
  auto&  here = *m_current;

    if(!next || next->m_number)
    {
      m_temporary_score = 0;

      exec.replace({*this,&stage::evaluate});
    }

  else
    if(next)
    {
      next->m_number = here.m_number    ;
                       here.m_number = 0;

      next->m_color = here.m_color                      ;
                      here.m_color = gbstd::colors::null;

      m_current = next;
    }
}


void
stage::
control_drop(gbstd::execution&  exec) noexcept
{
  static constexpr uint32_t  delay = 120;

  auto&  here = *m_current;

    if(gbstd::get_keys().test_opr())
    {
      gbstd::barrier_keys(120);
    }


    if(here.m_point.y && gbstd::get_keys().test_diu())
    {
      gbstd::barrier_keys(delay);

      control_by_cursor(exec,here.m_up);
    }

  else
    if(gbstd::get_keys().test_did())
    {
        if(here.m_point.x && gbstd::get_keys().test_dil())
        {
          control_by_cursor(exec,here.m_left);
        }

      else
        if((here.m_point.x < (m_width-1)) && gbstd::get_keys().test_dir())
        {
          control_by_cursor(exec,here.m_right);
        }


      control_by_cursor(exec,here.m_down);

      gbstd::barrier_keys(delay/3);
    }

  else
    {
        if(here.m_point.x && gbstd::get_keys().test_dil())
        {
          gbstd::barrier_keys(delay);

          control_by_cursor(exec,here.m_left);
        }

      else
        if((here.m_point.x < (m_width-1)) && gbstd::get_keys().test_dir())
        {
          gbstd::barrier_keys(delay);

          control_by_cursor(exec,here.m_right);
        }
    }


  exec.interrupt();
}


void
stage::
gameover(gbstd::execution&  exec) noexcept
{
    if(gbstd::get_keys().test_opr())
    {
      gbstd::barrier_keys(120);

      reset();

      exec.pop();
    }


  exec.interrupt();
}


void
stage::
start(gbstd::execution&  exec) noexcept
{
  auto  start_sp = &m_table[0][m_width/2];

    if(start_sp->m_number)
    {
      m_status.set(flags::gameover);

      exec.push({*this,&stage::gameover});
    }

  else
    {
      m_current = start_sp;

        for(;;)
        {
          auto  n = m_number_rand();

            if(n)
            {
              m_current->m_number = n;

              auto  cn = m_color_rand();

              m_current->m_color = (cn == 0)? gbstd::colors::yellow
                                  :(cn == 1)? gbstd::colors::cyan
                                  :(cn == 2)? gbstd::colors::magenta
                                  :(cn == 3)? gbstd::colors::black
                                          :   gbstd::colors::null;

              break;
            }
        }


      exec.push({*this,&stage::control_drop});
    }
}




void
stage::
initialize(gbstd::execution&  exec) noexcept
{
  gbstd::get_root_directory()["/video/sprites/FallBlockPazzle.spr00"].be_sprite().set_callback({*this,&stage::draw});

  m_alarm.assign(gbstd::get_root_directory()["/clocks/FallBlockPazzle.00"].be_clock());

  m_alarm.set_interval(100);

  m_color_rand.reset(0,3);
  m_number_rand.reset(3,3);

  exec.replace({*this,&stage::start});
}




void
stage::
draw(const gbstd::canvas&  cv) noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x <  m_width;  ++x){
      auto&  sp = m_table[y][x];

      int  state;

        if(m_status.test(flags::chink))
        {
          state = (!sp.test_flag(space::flags::erase) || (m_blink_counter&1))? 1:0;
        }

      else
        {
          state = (m_current == &sp)? 2:1;
        }


      sp.draw(cv,gbstd::g_font_height*2,state);
    }}


  auto  pt = m_current->m_point;

  char  buf[256];

  snprintf(buf,sizeof(buf),"SCORE: %5dちんちん",m_total_score);

  cv.draw_string(gbstd::colors::white,buf,0,0);

    if(m_status.test(flags::gameover))
    {
      cv.draw_string(gbstd::colors::white,"GAME OVER",cv.get_width()/2-(8*5),cv.get_height()/2);
    }

  else
    if(m_chink_counter)
    {
      snprintf(buf,sizeof(buf),"%dれんさ",m_chink_counter);

      cv.draw_string(gbstd::colors::white,buf,0,gbstd::g_font_height);
    }


  cv.draw_rectangle(gbstd::colors::white,0,gbstd::g_font_height*2,space::m_size*m_width,space::m_size*m_height);
}


game_information
stage::
get_information() noexcept
{
  game_information  inf;

  inf.screen_width  = space::m_size*m_width ;
  inf.screen_height = space::m_size*m_height;

  inf.title = "けいさんパズル・ゲーム";
  inf.category_name = "おちものパズル";
  inf.description = R"(おかれた　おなじいろの　ブロックが　ひだり　から　みぎ、うえ　から　したに　かさん　ひょうかされ、ごうけいが　6であるとき、きえる)";

  inf.boot = [](){
    gbstd::push_execution({*new stage,&stage::initialize});
  };


  return inf;
}




}




