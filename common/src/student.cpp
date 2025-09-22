#include "common/student.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>
#include <iostream>



bool is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool is_valid_date(const std::string& date_str) {
    std::istringstream iss(date_str);
    int d, m, y;
    char dot1, dot2;

    if (!(iss >> d >> dot1 >> m >> dot2 >> y) || dot1 != '.' || dot2 != '.') {
        return false;
    }

    if (iss.rdbuf()->in_avail() != 0) {
       return false;
    }

    if (m < 1 || m > 12) {
        return false;
    }
    if (d < 1) {
        return false;
    }

    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap(y)) {
        days_in_month[2] = 29;
    }

    if (d > days_in_month[m]) {
        return false;
    }

    return true;
}



bool Student::operator<(const Student& other) const {
    if (full_name != other.full_name) {
        return full_name < other.full_name;
    }
    return birth_date < other.birth_date;
}

bool Student::operator==(const Student& other) const {
    return full_name == other.full_name &&
           birth_date == other.birth_date;
}

struct StudentComparator {
    bool operator()(const Student& a, const Student& b) const {
        if (a.full_name != b.full_name) return a.full_name < b.full_name;
        return a.birth_date < b.birth_date;
    }
};

std::vector<Student> parse_student_files(const std::vector<std::string>& filenames) {
    std::vector<Student> students;
    for (const auto& filename : filenames) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            continue;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line.find_first_not_of(" \t") == std::string::npos) continue;

            std::vector<std::string> words;
            std::stringstream line_ss(line);
            std::string temp;
            while(line_ss >> temp) {
                words.push_back(temp);
            }

            if (words.size() < 3) continue;

            Student s;
            try {
                s.id = std::stoull(words[0]);
            } catch (const std::exception&) {
                continue; 
            }

            std::string birth_date_str = words.back();
            if (!is_valid_date(birth_date_str)) {
                std::cerr << "Warning: Invalid date in line, skipping: \"" << line << "\"" << std::endl;
                continue;
            }

            s.birth_date = birth_date_str;
            
            std::string full_name_str;
            for(size_t i = 1; i < words.size() - 1; ++i) {
                full_name_str += words[i] + (i == words.size() - 2 ? "" : " ");
            }
            s.full_name = full_name_str;

            students.push_back(s);
        }
    }
    return students;
}

std::vector<Student> merge_duplicates(std::vector<Student> students) {
    std::set<Student, StudentComparator> unique_students(students.begin(), students.end());
    std::vector<Student> result(unique_students.begin(), unique_students.end());
    return result;
}

void print_students(const std::vector<Student>& students) {
    for (const auto& s : students) {
        std::cout << "ID: " << s.id << ", Name: " << s.full_name
                  << ", Birth Date: " << s.birth_date << std::endl;
    }
}
