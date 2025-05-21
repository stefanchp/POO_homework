#ifndef Student_h
#define Student_h

#include "Curs.h"
#include "Exceptii.h"
#include "CursObligatoriu.h"
#include "CursOptional.h"

#include <vector>
#include <string>
#include <numeric> 
#include <algorithm> 
#include <iomanip> 
#include <memory>  

class Student {
private:
    std::string nume_student_;
    std::vector<std::shared_ptr<Curs>> cursuri_student_;
    const int id_student_;
    static int id_generator_student_;

public:
    Student(std::string nume) : nume_student_(nume), id_student_(++id_generator_student_) {
        if (nume.empty()) {
            throw ExceptieStudent("Numele studentului nu poate fi gol.");
        }
        // std::cout << "Constructor Student: " << nume_student_ << " (ID: " << id_student_ << ")\n";
    }

    Student(const Student& other)
        : nume_student_(other.nume_student_), id_student_(++id_generator_student_) /* id nou pentru fiecare student */ {
        // std::cout << "Copy Constructor Student: " << nume_student_ << " from " << other.nume_student_ << "\n";
        for (const auto& curs_ptr : other.cursuri_student_) {
            if (curs_ptr) {
                cursuri_student_.push_back(curs_ptr->clone()); // Deep copy cu clone
            }
        }
    }

    Student& operator=(Student other) {
        // std::cout << "Copy Assignment Student: " << this->nume_student_ << " = " << other.nume_student_ << "\n";
        swap(*this, other);
        return *this;
    }

    Student(Student&& other) noexcept
        : nume_student_(std::move(other.nume_student_)),
          cursuri_student_(std::move(other.cursuri_student_)),
          id_student_(other.id_student_)
    {
        // std::cout << "Move Constructor Student: " << nume_student_ << "\n";
    }

    Student& operator=(Student&& other) noexcept {
        // std::cout << "Move Assignment Student: " << this->nume_student_ << " = " << other.nume_student_ << "\n";
        if (this != &other) {
            nume_student_ = std::move(other.nume_student_);
            cursuri_student_ = std::move(other.cursuri_student_);
        }
        return *this;
    }

    // Friend swap
    friend void swap(Student& first, Student& second) noexcept {
        using std::swap;
        swap(first.nume_student_, second.nume_student_);
        swap(first.cursuri_student_, second.cursuri_student_);
    }

    ~Student() {
        // std::cout << "Destructor Student: " << nume_student_ << "\n";
    }

    void adauga_curs(const std::shared_ptr<Curs>& curs_nou) {
        if (!curs_nou) {
            throw ExceptieStudent("Cannot add a null course.");
        }
        auto it = std::find_if(cursuri_student_.begin(), cursuri_student_.end(),
                               [&](const std::shared_ptr<Curs>& c_existent) {
                                   return c_existent && c_existent->get_id_curs() == curs_nou->get_id_curs();
                               });
        if (it != cursuri_student_.end()) {
            throw CursDuplicat(curs_nou->get_nume());
        }
        cursuri_student_.push_back(curs_nou);
    }

    double calcul_medie_generala() const {
        if (cursuri_student_.empty()) {
            return 0.0;
        }
        double suma_note_ponderate = 0.0;
        int total_credite = 0;
        for (const auto& curs_ptr : cursuri_student_) {
            if (curs_ptr) {
                double nota_finala = curs_ptr->calcul_nota_finala();
                if (nota_finala >= 5.0) {
                    suma_note_ponderate += nota_finala * curs_ptr->get_credite();
                    total_credite += curs_ptr->get_credite();
                }
            }
        }
        return (total_credite > 0) ? (suma_note_ponderate / total_credite) : 0.0;
    }

    void afiseaza_cursuri_tip(const std::string& tip_nume_cautat) const {
        std::cout << "Cursuri de tip '" << tip_nume_cautat << "' pentru studentul " << nume_student_ << ":\n";
        bool found = false;
        for (const auto& curs_ptr : cursuri_student_) {
            if (!curs_ptr) continue;

            if (tip_nume_cautat == "Obligatoriu") {
                CursObligatoriu* co_ptr = dynamic_cast<CursObligatoriu*>(curs_ptr.get());
                if (co_ptr) {
                    std::cout << "  -> " << *co_ptr << "\n";
                    found = true;
                }
            } else if (tip_nume_cautat == "Optional") {
                CursOptional* copt_ptr = dynamic_cast<CursOptional*>(curs_ptr.get());
                if (copt_ptr) {
                    std::cout << "  -> " << *copt_ptr << "\n";
                    found = true;
                }
            }
        }
        if (!found) {
            std::cout << "  Niciun curs de acest tip gasit.\n";
        }
    }

    std::string get_nume() const { return nume_student_; }
    int get_id_student() const { return id_student_; }

    static int get_numar_total_studenti() { return id_generator_student_; }

    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "Student ID: " << student.id_student_ << ", Nume: " << student.nume_student_ << "\n";
        os << "  Medie Generala: " << std::fixed << std::setprecision(2) << student.calcul_medie_generala() << "\n";
        os << "  Cursuri Inscrise (" << student.cursuri_student_.size() << "):\n";
        for (const auto& curs_ptr : student.cursuri_student_) {
            if (curs_ptr) {
                os << "    - " << *curs_ptr << "\n";
            } else {
                os << "    - [Curs invalid/null]\n";
            }
        }
        return os;
    }
};

#endif