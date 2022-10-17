set(LIBRARY_NAME core)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

add_library(${LIBRARY_NAME})

target_link_libraries(${LIBRARY_NAME})

target_include_directories(${LIBRARY_NAME})

set_target_properties(${LIBRARY_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD ${MOZI_CXX_STANDARD}
    POSITION_INDEPENDENT_CODE ON
    )
