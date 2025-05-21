#ifndef Curs_h
#define Curs_h

#include <string>
#include <iostream>
#include <memory> 
#include "Exceptii.h"

class Curs {
protected:
    std::string nume_curs_;
    int credite_;
    const int id_curs_;
    static int id_generator_;
    
    virtual void afisare_specifica(std::ostream& os) const = 0;

    Curs(const Curs& other) :
        nume_curs_(other.nume_curs_),
        credite_(other.credite_),
        id_curs_(++id_generator_) // ID nou
    {}

    Curs& operator=(const Curs& other) = default; 

public:
    Curs(std::string nume, int credite) : nume_curs_(nume), credite_(credite), id_curs_(++id_generator_) {
        if (nume.empty()) {
            throw DateCursInvalide("Numele cursului nu poate fi gol.");
        }
        if (credite <= 0) {
            throw DateCursInvalide("Numarul de credite trebuie sa fie pozitiv. Primit: " + std::to_string(credite));
        }
        // std::cout << "Constructor Curs: " << nume_curs_ << " (ID: " << id_curs_ << ")\n";
    }

    virtual ~Curs() {
        // std::cout << "Destructor Curs: " << nume_curs_ << " (ID: " << id_curs_ << ")\n";
    }

    virtual double calcul_nota_finala() const = 0;
    virtual std::shared_ptr<Curs> clone() const = 0;

    std::string get_nume() const { return nume_curs_; }
    int get_credite() const { return credite_; }
    int get_id_curs() const { return id_curs_; }

    static int get_numar_total_cursuri_create() { return id_generator_; }

    friend std::ostream& operator<<(std::ostream& os, const Curs& curs) {
        os << "ID Curs: " << curs.id_curs_ << ", Nume: " << curs.nume_curs_ << ", Credite: " << curs.credite_;
        curs.afisare_specifica(os); // Polymorphic call
        return os;
    }
    
    Curs(Curs&& other) noexcept = default;
    Curs& operator=(Curs&& other) noexcept = default;
};

#endif