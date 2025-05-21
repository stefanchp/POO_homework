#ifndef Cursoptional_h
#define Cursoptional_h

#include "Curs.h"
#include <iomanip>

class CursOptional : public Curs {
private:
    double nota_proiect_;
    bool participare_activa_;

protected:
    void afisare_specifica(std::ostream& os) const override {
        os << ", Tip: Optional, Nota Proiect: " << std::fixed << std::setprecision(2) << nota_proiect_
           << ", Participare Activa: " << (participare_activa_ ? "Da" : "Nu");
    }

public:
    CursOptional(std::string nume, int credite, double nota_pr, bool participare)
        : Curs(nume, credite), nota_proiect_(nota_pr), participare_activa_(participare) {
        if (nota_pr < 1.0 || nota_pr > 10.0) {
            throw DateCursInvalide("Nota proiect invalida: " + std::to_string(nota_pr));
        }
        // std::cout << "Constructor CursOptional: " << nume_curs_ << "\n";
    }

    ~CursOptional() override {
        // std::cout << "Destructor CursOptional: " << nume_curs_ << "\n";
    }

    double calcul_nota_finala() const override {
        double nota_finala = nota_proiect_;
        if (participare_activa_ && nota_finala < 10.0) {
            nota_finala += 1.0;
            if (nota_finala > 10.0) nota_finala = 10.0;
        }
        return (nota_finala < 5.0) ? 4.0 : nota_finala;
    }

    std::shared_ptr<Curs> clone() const override {
        return std::make_shared<CursOptional>(*this);
    }
};

#endif