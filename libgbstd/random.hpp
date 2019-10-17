#ifndef gbstd_random_HPP
#define gbstd_random_HPP


#include<random>




namespace gbstd{




void  initialize_random() noexcept;


extern std::default_random_engine
g_random_number_engine;


class
normal_rand
{
  std::normal_distribution<>  m_dist;
//mean平均
//stddev標準偏差(+-)

public:
  normal_rand() noexcept{}
  normal_rand(double  mean, double  stddev) noexcept{reset(mean,stddev);}

  void  reset(double  mean, double  stddev) noexcept{m_dist = std::normal_distribution<>(mean,stddev);}

  double  operator()() noexcept{return m_dist(g_random_number_engine);}

};


class
uniform_rand
{
  std::uniform_int_distribution<>  m_dist;

public:
  uniform_rand() noexcept{}
  uniform_rand(int  min, int  max) noexcept{reset(min,max);}

  void  reset(int  min, int  max) noexcept{m_dist = std::uniform_int_distribution<>(min,max);}

  int  operator()() noexcept{return m_dist(g_random_number_engine);}

};


class
uniform_real_rand
{
  std::uniform_real_distribution<>  m_dist;

public:
  uniform_real_rand() noexcept{}
  uniform_real_rand(double  min, double  max) noexcept{reset(min,max);}

  void  reset(double  min, double  max) noexcept{m_dist = std::uniform_real_distribution<>(min,max);}

  double  operator()() noexcept{return m_dist(g_random_number_engine);}

};




}




#endif




