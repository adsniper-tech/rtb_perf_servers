include_directories(.)

set(APP_NAME udp_server)
add_compile_definitions(APP_NAME="${APP_NAME}")

add_executable(${APP_NAME} main.cpp Config.h Config.cpp)
target_link_libraries(${APP_NAME} ${SNIPER_LIBRARIES} ${PROJECT_LIBS})
