cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package (Threads)
add_executable (grep "main.cpp" "grep.cpp" "grep.h" "is_binary.cpp" "is_binary.h" "lock_free_queue.h")
target_link_libraries (grep Threads::Threads)