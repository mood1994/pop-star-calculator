cmake_minimum_required (VERSION 2.6)
project (pop_star_calculator)
set (POP_STAR_CALC_VERSION_MAJOR 0)
set (POP_STAR_CALC_VERSION_MINOR 0)

set (SRC_LIST main.cpp block.cpp star.cpp plan.cpp monitor.cpp)

add_executable(pop_star_calculator ${SRC_LIST})
