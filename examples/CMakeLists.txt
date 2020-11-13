add_executable(base base/main.cpp)
target_link_libraries(base mango)

add_executable(cube cube/App.cpp cube/App.hpp cube/main.cpp)
target_link_libraries(cube mango)

#add_executable(gltf gltf/App.cpp gltf/App.hpp gltf/main.cpp)
#target_link_libraries(gltf mango)

#add_executable(compute compute/main.cpp)
#target_link_libraries(compute mango)

add_executable(procedure procedure/App.cpp procedure/App.hpp procedure/main.cpp)
target_link_libraries(procedure mango)