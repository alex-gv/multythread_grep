# CMakeList.txt: проект CMake для grep; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (grep "grep.cpp" "grep.h" "is_binary.cpp" "is_binary.h" "lock_free_queue.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET grep PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
