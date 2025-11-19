set(SOURCES
    client.cpp 
    srv.cpp
)

add_executable(client client.cpp)
add_executable(srv srv.cpp)

target_link_libraries(client PRIVATE
    ${LUAJIT_LIBRARY}
    ${ZMQ_LIBRARY}
    pthread
    common
)

target_link_libraries(srv PRIVATE
    ${LUAJIT_LIBRARY}
    ${ZMQ_LIBRARY}
    ${CPR_LIBRARY}
    ${CURL_LIBRARY}
    pthread
    common
)

install(TARGETS
    client srv
    RUNTIME DESTINATION bin
)

install(FILES
    ${COMMON_DIR}/json.hpp
    ${PROTO_HDRS}
    DESTINATION include/common
)
