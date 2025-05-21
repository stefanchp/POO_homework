#ifndef Cursobligatoriu_h
#define Cursobligatoriu_h

#include "Curs.h"
#include <iomanip>

class CursObligatoriu : public Curs {
private:
    double nota_examen_;
    double nota_laborator_;

protected:
    void afisare_specifica(std::ostream& os) const override {
        os << ", Tip: Obligatoriu, Nota Examen: " << std::fixed << std::setprecision(2) << nota_examen_
           << ", Nota Laborator: " << nota_laborator_;
    }

public:
    CursObligatoriu(std::string nume, int credite, double nota_ex, double nota_lab)
        : Curs(nume, credite), nota_examen_(nota_ex), nota_laborator_(nota_lab) {
        if (nota_ex < 1.0 || nota_ex > 10.0) {
            throw DateCursInvalide("Nota examen invalida: " + std::to_string(nota_ex));
        }
        if (nota_lab < 1.0 || nota_lab > 10.0) {
            throw DateCursInvalide("Nota laborator invalida: " + std::to_string(nota_lab));
        }
        // std::cout << "Constructor CursObligatoriu: " << nume_curs_ << "\n";
    }

    ~CursObligatoriu() override {
        // std::cout << "Destructor CursObligatoriu: " << nume_curs_ << "\n";
    }

    double calcul_nota_finala() const override {
        double nota_finala = 0.6 * nota_examen_ + 0.4 * nota_laborator_;
        return (nota_finala < 5.0) ? 4.0 : nota_finala; // Simplified: if less than 5, it's 4.
    }

    std::shared_ptr<Curs> clone() const override {
        return std::make_shared<CursObligatoriu>(*this);
    }
};

#endif