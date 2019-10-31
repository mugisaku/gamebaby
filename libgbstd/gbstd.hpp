#ifndef GBSTD_hpp_was_included
#define GBSTD_hpp_was_included


#include<cstdint>
#include<cstdio>
#include<vector>
#include"libgbstd/gpfs.hpp"




namespace gbstd{




void  push_execution(callback_wrapper  cb) noexcept;
void   pop_execution(                    ) noexcept;


uint32_t     get_time(           ) noexcept;
void      update_time(uint32_t  t) noexcept;


const key_state&  get_modified_keys(                      ) noexcept;
const key_state&   get_pressed_keys(                      ) noexcept;
const key_state&           get_keys(                      ) noexcept;
void                    update_keys(const key_state&  keys) noexcept;

void  barrier_keys(uint32_t  interval=240) noexcept;

void   update_point(point  pt) noexcept;
liner  make_liner() noexcept;

extern std::vector<uint8_t>  g_dropped_file;

gpfs::directory&  get_root_directory() noexcept;



}


#endif




