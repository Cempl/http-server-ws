set(CURTARGET "SSL_SERVER")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${CURTARGET}
	SslServer.h
	SslServer.cpp
    Transport.h
    Transport.cpp
)

target_link_libraries(${CURTARGET} Qt5::Network)
target_link_libraries(${CURTARGET} HTTP)
