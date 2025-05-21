#ifndef Facultate_h
#define Facultate_h

#include "Student.h"
#include <vector>
#include <string>
#include <algorithm>

class Facultate {
private:
    std::string nume_facultate_;
    std::vector<Student> studenti_;

public:
    explicit Facultate(std::string nume) : nume_facultate_(nume) {
        if (nume.empty()){
            throw std::invalid_argument("Numele facultatii nu poate fi gol.");
        }
        // std::cout << "Constructor Facultate: " << nume_facultate_ << "\n";
    }

    // Facultate va folosi constructorii default de copiere

    void adauga_student(const Student& student) {
        // Verificam duplicatele
        auto it = std::find_if(studenti_.begin(), studenti_.end(), 
                                [&](const Student& s_existent) {
                                   return s_existent.get_id_student() == student.get_id_student();
                               });
        if (it != studenti_.end()) {
            std::cerr << "Warning: Studentul " << student.get_nume() 
                      << " (ID: " << student.get_id_student() 
                      << ") este deja in facultate.\n";
            return;
        }
        studenti_.push_back(student);
    }
    
    void adauga_student(Student&& student) { // Overload for rvalue reference
        auto it = std::find_if(studenti_.begin(), studenti_.end(),
                               [&](const Student& s_existent) {
                                   return s_existent.get_id_student() == student.get_id_student();
                               });
        if (it != studenti_.end()) {
            std::cerr << "Warning: Studentul " << student.get_nume() 
                      << " (ID: " << student.get_id_student() 
                      << ") este deja in facultate.\n";
            return;
        }
        studenti_.push_back(std::move(student)); 
    }

    int get_numar_studenti()
    {
        return studenti_.size();
    }


    friend std::ostream& operator<<(std::ostream& os, const Facultate& facultate) {
        os << "Facultate: " << facultate.nume_facultate_ << "\n";
        os << "----------------------------------------------------------\n";
        os << "Numar studenti: " << facultate.studenti_.size() << "\n";
        for (const auto& student : facultate.studenti_) {
            os << student << "---------------------\n";
        }
        return os;
    }
};

#endif