include_directories(.)

set(APP_NAME tcp_server)
add_compile_definitions(APP_NAME="${APP_NAME}")

add_executable(tcp_server server.cpp Config.h Config.cpp)
target_link_libraries(tcp_server ${SNIPER_LIBRARIES} ${PROJECT_LIBS})
