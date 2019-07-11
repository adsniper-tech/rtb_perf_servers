include_directories(.)
include_directories(${SNIPER_INCLUDE_DIRS})

add_compile_definitions(GIT_SHA1="${GIT_SHA1}")
message("GIT Revision=${GIT_SHA1}")


set(PROJECT_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${BOOST_LIBRARIES}
    ${CONFIG++_LIBRARY}
    backtrace dl
    fmt::fmt
    stdc++fs.a
    tcmalloc
    )

add_subdirectory(tcp)
add_subdirectory(udp)
