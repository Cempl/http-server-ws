set(CURTARGET "SSL_SERVER")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${CURTARGET}
	Response.h
	Response.cpp
	WSLexer.h
	WSLexer.cpp
)

target_link_libraries(${CURTARGET} Qt5::Network)