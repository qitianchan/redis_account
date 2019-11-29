cmake_minimum_required(VERSION 3.13)

include_directories("/usr/local/include/")

link_directories("/usr/local/lib")

link_libraries(hiredis)
add_executable(account account.c)

add_executable(main main.c)
