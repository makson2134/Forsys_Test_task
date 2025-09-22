#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <httplib.h>
#include "common/student.h"

void run_zmq_publisher(const std::vector<Student>& students) {
    nlohmann::json students_json = students;
    const std::string json_str = students_json.dump();

    zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5555");
    
    std::cout << "ZMQ Publisher started on tcp://*:5555. Publishing data every 2 seconds." << std::endl;

    while (true) {
        publisher.send(zmq::str_buffer("students"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer(json_str));
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void run_http_server(const std::vector<Student>& students) {
    httplib::Server svr;
    nlohmann::json students_json = students;
    const std::string json_str = students_json.dump();

    svr.Get("/students", [&](const httplib::Request& req, httplib::Response& res) {
        std::cout << "HTTP: Received GET /students request from " << req.remote_addr << std::endl;
        res.set_content(json_str, "application/json");
    });

    std::cout << "HTTP Server attempting to start on http://localhost:8080" << std::endl;
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "FATAL HTTP ERROR: Could not listen on port 8080. Is it already in use by another application?" << std::endl;
    }
}

int main() {
    auto students = parse_student_files({"data/student_file_1.txt", "data/student_file_2.txt"});
    auto unique_students = merge_duplicates(students);

    std::thread zmq_thread(run_zmq_publisher, std::ref(unique_students));
    std::thread http_thread(run_http_server, std::ref(unique_students));

    zmq_thread.join();
    http_thread.join();

    return 0;
}
