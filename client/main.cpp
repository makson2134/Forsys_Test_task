#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <httplib.h>
#include "common/student.h"

void run_zmq_subscriber() {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555");
    subscriber.set(zmq::sockopt::subscribe, "students");

    std::cout << "ZMQ Subscriber connected to tcp://localhost:5555" << std::endl;

    zmq::message_t topic;
    zmq::message_t content;
    
    subscriber.recv(topic, zmq::recv_flags::none);
    subscriber.recv(content, zmq::recv_flags::none);

    auto received_students = nlohmann::json::parse(content.to_string()).get<std::vector<Student>>();

    std::sort(received_students.begin(), received_students.end());

    std::cout << "\n--- Received and Sorted Students (ZMQ) ---" << std::endl;
    print_students(received_students);
}

void run_http_client() {
    httplib::Client cli("localhost", 8080);
    auto res = cli.Get("/students");

    if (res && res->status == 200) {
        auto received_students = nlohmann::json::parse(res->body).get<std::vector<Student>>();
        std::sort(received_students.begin(), received_students.end());
        std::cout << "\nReceived and Sorted Students (HTTP)" << std::endl;
        print_students(received_students);
    } else {
        std::cerr << "Error: Failed to get students via HTTP." << std::endl;
    }
}

int main() {
    std::string choice;
    while (true) {
        std::cout << "\nEnter command (zmq, http, exit): ";
        std::cin >> choice;

        if (choice == "zmq") {
            run_zmq_subscriber();
        } else if (choice == "http") {
            run_http_client();
        } else if (choice == "exit") {
            break;
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }
    return 0;
}
