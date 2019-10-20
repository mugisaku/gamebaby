#ifndef InterLanguageCommunication_hpp_was_included
#define InterLanguageCommunication_hpp_was_included


#include<cstdint>
#include<cstddef>
#include<vector>




namespace gbstd{




void  initialize_javascript() noexcept;

void  show_github_link() noexcept;
void  show_twitter_link() noexcept;
void  set_caption(const char*  s) noexcept;
void  set_description(const char*  s) noexcept;

void    update_blob(const uint8_t*  ptr, size_t  size) noexcept;
void  download_blob(const char*  filename) noexcept;

int  get_number_of_dropped_files() noexcept;

std::vector<uint8_t>  pop_front_dropped_file() noexcept;




}




#endif




