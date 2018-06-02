set(CURTARGET "sslserver_private")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${CURTARGET}
    SslServer.h
    SslServer.cpp
)

#target_link_libraries(${CURTARGET} PUBLIC sslserver_interfaces)
#target_link_libraries(${CURTARGET} PUBLIC main)
