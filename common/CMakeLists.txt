add_library(common STATIC
    src/student.cpp
)

target_include_directories(common PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(common PUBLIC nlohmann_json::nlohmann_json)
