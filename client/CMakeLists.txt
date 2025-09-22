add_executable(client main.cpp)

target_link_libraries(client
    PRIVATE
    common
    cppzmq
    nlohmann_json::nlohmann_json
    httplib::httplib
)
