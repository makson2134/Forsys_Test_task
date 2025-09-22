#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>
#include <nlohmann/json.hpp>

struct Student {
    uint64_t id;
    std::string full_name;
    std::string birth_date;

    bool operator<(const Student& other) const;
    bool operator==(const Student& other) const;
};

inline void to_json(nlohmann::json& j, const Student& s) {
    j = nlohmann::json{{"id", s.id}, {"full_name", s.full_name}, {"birth_date", s.birth_date}};
}

inline void from_json(const nlohmann::json& j, Student& s) {
    j.at("id").get_to(s.id);
    j.at("full_name").get_to(s.full_name);
    j.at("birth_date").get_to(s.birth_date);
}

std::vector<Student> parse_student_files(const std::vector<std::string>& filenames);

std::vector<Student> merge_duplicates(std::vector<Student> students);

void print_students(const std::vector<Student>& students);
