add_executable(base base/main.cpp)
target_link_libraries(base mango)

add_executable(cube cube/App.cpp cube/App.hpp cube/main.cpp)
target_link_libraries(cube mango)