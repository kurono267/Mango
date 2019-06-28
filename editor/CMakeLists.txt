set(APP app/main.cpp app/App.cpp)

add_executable(editor ${APP})
target_link_libraries(editor mango)