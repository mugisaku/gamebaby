

set(SOURCES
  sns_sha256.cpp
  sns_tester.cpp
  sns_index_list.cpp
  sns_record.cpp
  sns_record_table.cpp
  sns_article.cpp
  sns_account.cpp
  sns_account_observer.cpp
  sns_platform.cpp
  sns_timeline.cpp
  sns_timeline_node.cpp
)


include_directories(./ ../)
add_library("sns" ${SOURCES})
add_executable("test_sns" "test_sns.cpp")
add_executable("test_sha256" "test_sha256.cpp")
target_link_libraries("test_sns" "sns")
target_link_libraries("test_sha256" "sns")




