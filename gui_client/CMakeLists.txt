if(WIN32)
    add_executable(gui_client WIN32
        main.cpp
        mainwindow.cpp
        mainwindow.h
        Client_form.ui
    )
else()
    add_executable(gui_client
        main.cpp
        mainwindow.cpp
        mainwindow.h
        Client_form.ui
    )
endif()

target_link_libraries(gui_client
    PRIVATE
    common
    cppzmq
    nlohmann_json::nlohmann_json
    httplib::httplib
    Qt6::Core
    Qt6::Widgets
)
