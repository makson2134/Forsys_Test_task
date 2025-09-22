add_executable(server main.cpp)

target_link_libraries(server
    PRIVATE
    common
    cppzmq
    nlohmann_json::nlohmann_json
    httplib::httplib
)

add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/data"
    "$<TARGET_FILE_DIR:server>/data"
)
