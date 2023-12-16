add_library(utils src/utils.cpp)
target_include_directories(utils PUBLIC include)
target_link_libraries(utils PUBLIC MPI::MPI_CXX)
